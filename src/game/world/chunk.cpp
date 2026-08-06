#include "chunk.h"

#include "game/block/blockDictionary.h"

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

void Chunk::buildMesh(const ChunkMap& chunkMap) {
    // No mesh can be built if chunk hasn't been populated
    if (state == ChunkState::EMPTY) {
        return;
    }

    // Get adjacent chunks
    auto leftChunkIt = chunkMap.find(make_pair(chunkPosition.x - 1, chunkPosition.z));
    auto rightChunkIt = chunkMap.find(make_pair(chunkPosition.x + 1, chunkPosition.z));
    auto backChunkIt = chunkMap.find(make_pair(chunkPosition.x, chunkPosition.z - 1));
    auto frontChunkIt = chunkMap.find(make_pair(chunkPosition.x, chunkPosition.z + 1));

    // Check for error case where neighboring chunks aren't defined
    if (leftChunkIt == chunkMap.end() || rightChunkIt == chunkMap.end() ||
        backChunkIt == chunkMap.end() || frontChunkIt == chunkMap.end()) {
        return;
    }

    auto leftChunk = leftChunkIt->second.get();
    auto rightChunk = rightChunkIt->second.get();
    auto backChunk = backChunkIt->second.get();
    auto frontChunk = frontChunkIt->second.get();

    // Assert that all neighboring chunks are populated
    if (leftChunk->getChunkState() == ChunkState::EMPTY || rightChunk->getChunkState() == ChunkState::EMPTY ||
        backChunk->getChunkState() == ChunkState::EMPTY || frontChunk->getChunkState() == ChunkState::EMPTY) {
        return;
    }

    state = ChunkState::POPULATED;
    vector<Vertex> vertices(vertexCount);
    vector<Vertex> transparentVertices(transparentVertexCount);
    auto dictionary = BlockDictionary::getInstance();

    for (int bx = 0; bx < CHUNK_SIZE_X; ++bx) {
        for (int by = 0; by < CHUNK_SIZE_Y; ++by) {
            for (int bz = 0; bz < CHUNK_SIZE_Z; ++bz) {
                auto block = data.get(getIndex(bx, by, bz));

                // Generate no geometry for air blocks
                if (block == 0) {
                    continue;
                }

                auto type = dictionary->get(block);
                auto localPosition = u8vec3(bx, by, bz);

                // Check if transparent block is a billboard
                if (type.isBillboard) {
                    addBillboard(vertices, transparentVertices, type, localPosition);
                    continue;
                }

                // Getting adjacent blocks
                // We could calculate world position, then use methods in world to get these much cleaner
                // The issue is the bounds check makes it significantly slower to build a mesh
                auto leftBlock = bx > 0
                                     ? getBlock(bx - 1, by, bz)
                                     : leftChunk->getBlock(CHUNK_SIZE_X - 1, by, bz);
                auto rightBlock = bx < CHUNK_SIZE_X - 1
                                      ? getBlock(bx + 1, by, bz)
                                      : rightChunk->getBlock(0, by, bz);
                auto backBlock = bz > 0
                                     ? getBlock(bx, by, bz - 1)
                                     : backChunk->getBlock(bx, by, CHUNK_SIZE_Z - 1);
                auto frontBlock = bz < CHUNK_SIZE_Z - 1
                                      ? getBlock(bx, by, bz + 1)
                                      : frontChunk->getBlock(bx, by, 0);
                auto bottomBlock = by > 0 ? getBlock(bx, by - 1, bz) : 0;
                auto topBlock = by < CHUNK_SIZE_Y - 1 ? getBlock(bx, by + 1, bz) : 0;

                if (leftBlock != block && isVisibleFace(type, dictionary->get(leftBlock))) {
                    addFace(vertices, transparentVertices, type, BlockFace::LEFT, localPosition);
                }
                if (rightBlock != block && isVisibleFace(type, dictionary->get(rightBlock))) {
                    addFace(vertices, transparentVertices, type, BlockFace::RIGHT, localPosition);
                }
                if (backBlock != block && isVisibleFace(type, dictionary->get(backBlock))) {
                    addFace(vertices, transparentVertices, type, BlockFace::BACK, localPosition);
                }
                if (frontBlock != block && isVisibleFace(type, dictionary->get(frontBlock))) {
                    addFace(vertices, transparentVertices, type, BlockFace::FRONT, localPosition);
                }
                if (bottomBlock != block && isVisibleFace(type, dictionary->get(bottomBlock))) {
                    addFace(vertices, transparentVertices, type, BlockFace::BOTTOM, localPosition);
                }
                if (topBlock != block && isVisibleFace(type, dictionary->get(topBlock))) {
                    addFace(vertices, transparentVertices, type, BlockFace::TOP, localPosition);
                }
            }
        }
    }

    vertexCount = static_cast<int>(vertices.size());
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

void Chunk::write(vector<char>& byteData) const {
    for (int x = 0; x < CHUNK_SIZE_X; ++x) {
        for (int y = 0; y < CHUNK_SIZE_Y; ++y) {
            for (int z = 0; z < CHUNK_SIZE_Z; ++z) {
                const auto block = data.get(getIndex(x, y, z));
                auto index = (x * CHUNK_SIZE_Z * CHUNK_SIZE_Y + y * CHUNK_SIZE_Z + z) * 2;
                std::memcpy(byteData.data() + index, &block, sizeof(block));
            }
        }
    }
}

void Chunk::load(ifstream& in) {
    if (state != ChunkState::EMPTY) {
        return;
    }

    for (int x = 0; x < CHUNK_SIZE_X; ++x) {
        for (int y = 0; y < CHUNK_SIZE_Y; ++y) {
            for (int z = 0; z < CHUNK_SIZE_Z; ++z) {
                uint16_t id;
                in.read(reinterpret_cast<char*>(&id), sizeof(id));
                data.set(getIndex(x, y, z), id);
            }
        }
    }

    state = ChunkState::POPULATED;
}

BlockID Chunk::getBlock(int x, int y, int z) const {
    return data.get(getIndex(x, y, z));
}

void Chunk::setBlock(int x, int y, int z, const BlockID& block) {
    data.set(getIndex(x, y, z), block);
}

ChunkState Chunk::getChunkState() const {
    return state;
}

const ivec3& Chunk::getChunkPosition() const {
    return chunkPosition;
}

bool Chunk::isValidBlockPosition(const int x, const int y, const int z) {
    return x >= 0 && x < CHUNK_SIZE_X &&
        y >= 0 && y < CHUNK_SIZE_Y &&
        z >= 0 && z < CHUNK_SIZE_Z;
}

void Chunk::addFace(
    vector<Vertex>& vertices, vector<Vertex>& transparentVertices, const BlockType& type, const BlockFace face,
    const u8vec3& position) {
    for (const auto& vertex : Block::blockFaceVertices[face]) {
        auto v = Vertex(vertex.position + position, vertex.uv, type.getLayer(face));

        if (type.opaque) {
            vertices.push_back(v);
        } else {
            transparentVertices.push_back(v);
        }
    }
}

void Chunk::addBillboard(
    vector<Vertex>& vertices, vector<Vertex>& transparentVertices, const BlockType& type, const u8vec3& position) {
    for (const auto& vertex : Block::billboardVertices) {
        auto v = Vertex(vertex.position + position, vertex.uv, type.getLayer(BlockFace::FRONT));

        if (type.opaque) {
            vertices.push_back(v);
        } else {
            transparentVertices.push_back(v);
        }
    }
}

void Chunk::setChunkState(const ChunkState newState) {
    state = newState;
}

int Chunk::getIndex(int x, int y, int z) {
    return x * CHUNK_SIZE_Y * CHUNK_SIZE_Z + y * CHUNK_SIZE_Z + z;
}

bool Chunk::isVisibleFace(const BlockType& a, const BlockType& b) {
    return a.opaque != b.opaque || (!a.opaque && !b.opaque && a.id != b.id);
}
