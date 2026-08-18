#include "chunk.h"

#include "log.h"
#include "game/world/world.h"
#include "game/block/blockDictionary.h"
#include "mesh/faceCullingMesher.h"
#include "mesh/greedyMesher.h"

Chunk::Chunk(const int x, const int z, const shared_ptr<ElementBuffer>& ebo) : chunkPosition(x, 0, z) {
    // Initialize VertexBuffer attributes
    vao.bind();
    vbo.bind();
    ebo->bind();
    vbo.vertexAttribIPointer(0, 3, GL_UNSIGNED_BYTE, sizeof(Vertex), nullptr);
    vbo.vertexAttribIPointer(1, 2, GL_UNSIGNED_BYTE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, uv)));
    vbo.vertexAttribIPointer(2, 1, GL_UNSIGNED_BYTE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, layer)));
    VertexArray::unbind();

    transparentVAO.bind();
    transparentVBO.bind();
    ebo->bind();
    transparentVBO.vertexAttribIPointer(0, 3, GL_UNSIGNED_BYTE, sizeof(Vertex), nullptr);
    transparentVBO.vertexAttribIPointer(1, 2, GL_UNSIGNED_BYTE, sizeof(Vertex),
                                        reinterpret_cast<void*>(offsetof(Vertex, uv)));
    transparentVBO.vertexAttribIPointer(2, 1, GL_UNSIGNED_BYTE, sizeof(Vertex),
                                        reinterpret_cast<void*>(offsetof(Vertex, layer)));
    VertexArray::unbind();
}

void Chunk::buildMesh(const World& world) {
    // Timing chunk builds for development
    auto start = std::chrono::steady_clock::now();

    // Check if adjacent chunks are populated
    array<Chunk*, 9> neighbors = {};
    for (int x = 0; x < 3; ++x) {
        for (int z = 0; z < 3; ++z) {
            auto index = x * 3 + z;
            auto cx = x - 1 + chunkPosition.x;
            auto cz = z - 1 + chunkPosition.z;

            if (x == 1 && z == 1) {
                neighbors[index] = this;
                continue;
            }

            auto chunk = world.getChunk(cx, cz);
            if (chunk == nullptr || chunk->getChunkState() == ChunkState::EMPTY) {
                return;
            }
            neighbors[index] = chunk;
        }
    }

    state = ChunkState::POPULATED;

    // Fetch all blocks into buffer for cheaper lookups
    array<BlockID, (CHUNK_SIZE_X + 2) * (CHUNK_SIZE_Z + 2) * (CHUNK_SIZE_Y + 2)> buildCache{};
    for (int bx = 0; bx < CHUNK_SIZE_X + 2; ++bx) {
        for (int by = 0; by < CHUNK_SIZE_Y; ++by) {
            for (int bz = 0; bz < CHUNK_SIZE_Z + 2; ++bz) {
                auto cx = bx > CHUNK_SIZE_X ? 2 : bx > 0 ? 1 : 0;
                auto cz = bz > CHUNK_SIZE_Z ? 2 : bz > 0 ? 1 : 0;
                auto index = Mesher::getCacheIndex(bx - 1, by, bz - 1);

                if (cx == 1 && cz == 1) {
                    buildCache[index] = getBlock(bx - 1, by, bz - 1);
                    continue;
                }

                auto chunk = neighbors[cx * 3 + cz];
                auto localX = cx == 1 ? bx - 1 : cx == 0 ? CHUNK_SIZE_X - 1 : 0;
                auto localZ = cz == 1 ? bz - 1 : cz == 0 ? CHUNK_SIZE_Z - 1 : 0;
                buildCache[index] = chunk->getBlock(localX, by, localZ);
            }
        }
    }

    // auto mesher = GreedyMesher(vertexCount, transparentVertexCount);
    auto mesher = FaceCullingMesher(vertexCount, transparentVertexCount);
    mesher.build(buildCache);

    const vector<Vertex>& vertices = mesher.getVertices();
    vertexCount = static_cast<int>(vertices.size());

    const vector<Vertex>& transparentVertices = mesher.getTransparentVertices();
    transparentVertexCount = static_cast<int>(transparentVertices.size());

    // Copy vertices to VBO
    if (!vertices.empty()) {
        vbo.bufferData(vertexCount * sizeof(Vertex), &vertices.front(), GL_STATIC_DRAW);
    }

    if (!transparentVertices.empty()) {
        transparentVBO.bufferData(transparentVertexCount * sizeof(Vertex), &transparentVertices.front(),
                                  GL_STATIC_DRAW);
    }

    state = ChunkState::BUILT;

    auto end = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    LOG_DEBUG("Chunk build took {} microseconds", elapsed.count());
}

void Chunk::render() const {
    if (state == ChunkState::BUILT && vertexCount > 0) {
        vao.bind();
        glDrawElements(GL_TRIANGLES, vertexCount / 4 * 6, GL_UNSIGNED_INT, nullptr);
        VertexArray::unbind();
    }
}

void Chunk::renderTransparent() const {
    if (state == ChunkState::BUILT && transparentVertexCount > 0) {
        transparentVAO.bind();
        glDrawElements(GL_TRIANGLES, transparentVertexCount / 4 * 6, GL_UNSIGNED_INT,
                       nullptr);
        VertexArray::unbind();
    }
}

void Chunk::write(RegionFile* regionFile) const {
    regionFile->write(chunkPosition.x, chunkPosition.z, data);
}

void Chunk::load(RegionFile* regionFile) {
    if (state != ChunkState::EMPTY) {
        LOG_WARN("Attempted to load chunk {} {} twice", chunkPosition.x, chunkPosition.z);
        return;
    }

    auto loaded = regionFile->load(chunkPosition.x, chunkPosition.z);
    if (loaded.has_value()) {
        data = loaded.value();
        state = ChunkState::POPULATED;
    }
}

BlockID Chunk::getBlock(const int x, const int y, const int z) const {
    return data.get(getIndex(x, y, z));
}

void Chunk::setBlock(const int x, const int y, const int z, const BlockID& block) {
    data.set(getIndex(x, y, z), block);
}

ChunkState Chunk::getChunkState() const {
    return state;
}

const ivec3& Chunk::getChunkPosition() const {
    return chunkPosition;
}

int Chunk::getVertexCount() const {
    return vertexCount + transparentVertexCount;
}

bool Chunk::isValidBlockPosition(const int x, const int y, const int z) {
    return x >= 0 && x < CHUNK_SIZE_X &&
        y >= 0 && y < CHUNK_SIZE_Y &&
        z >= 0 && z < CHUNK_SIZE_Z;
}

void Chunk::setChunkState(const ChunkState newState) {
    state = newState;
}
