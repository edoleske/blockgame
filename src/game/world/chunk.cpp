#include "chunk.h"

Chunk::Chunk(const int x, const int z, const shared_ptr<ElementBuffer>& ebo) : chunkPosition(x, 0, z), ebo(ebo) {
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
    vertices.clear();
    transparentVertices.clear();

    for (int bx = 0; bx < CHUNK_SIZE_X; ++bx) {
        for (int by = 0; by < CHUNK_SIZE_Y; ++by) {
            for (int bz = 0; bz < CHUNK_SIZE_Z; ++bz) {
                auto type = blocks[getIndex(bx, by, bz)].getType();

                // Generate no geometry for air blocks
                if (type == 0) {
                    continue;
                }

                auto localPosition = u8vec3(bx, by, bz);

                // Check if transparent block is a billboard
                if (type.isBillboard) {
                    addBillboard(type, localPosition);
                    continue;
                }

                // Getting adjacent blocks
                // We could calculate world position, then use methods in world to get these much cleaner
                // The issue is the bounds check makes it significantly slower to build a mesh
                auto leftBlock = bx > 0
                                     ? getBlock(bx - 1, by, bz).getType()
                                     : leftChunk->getBlock(CHUNK_SIZE_X - 1, by, bz).getType();
                auto rightBlock = bx < CHUNK_SIZE_X - 1
                                      ? getBlock(bx + 1, by, bz).getType()
                                      : rightChunk->getBlock(0, by, bz).getType();
                auto backBlock = bz > 0
                                     ? getBlock(bx, by, bz - 1).getType()
                                     : backChunk->getBlock(bx, by, CHUNK_SIZE_Z - 1).getType();
                auto frontBlock = bz < CHUNK_SIZE_Z - 1
                                      ? getBlock(bx, by, bz + 1).getType()
                                      : frontChunk->getBlock(bx, by, 0).getType();
                auto bottomBlock = by > 0 ? getBlock(bx, by - 1, bz).getType() : BlockType();
                auto topBlock = by < CHUNK_SIZE_Y - 1 ? getBlock(bx, by + 1, bz).getType() : BlockType();

                if (isVisibleFace(type, leftBlock)) {
                    addFace(type, BlockFace::LEFT, localPosition);
                }
                if (isVisibleFace(type, rightBlock)) {
                    addFace(type, BlockFace::RIGHT, localPosition);
                }
                if (isVisibleFace(type, backBlock)) {
                    addFace(type, BlockFace::BACK, localPosition);
                }
                if (isVisibleFace(type, frontBlock)) {
                    addFace(type, BlockFace::FRONT, localPosition);
                }
                if (isVisibleFace(type, bottomBlock)) {
                    addFace(type, BlockFace::BOTTOM, localPosition);
                }
                if (isVisibleFace(type, topBlock)) {
                    addFace(type, BlockFace::TOP, localPosition);
                }
            }
        }
    }

    // Copy vertices to VBO
    if (!vertices.empty()) {
        vao.bind();
        vbo.bind();
        vbo.bufferData(vertices.size() * sizeof(Vertex), &vertices.front(), GL_STATIC_DRAW);
        VertexArray::unbind();
    }

    if (!transparentVertices.empty()) {
        transparentVAO.bind();
        transparentVBO.bind();
        transparentVBO.bufferData(transparentVertices.size() * sizeof(Vertex), &transparentVertices.front(),
                                  GL_STATIC_DRAW);
        VertexArray::unbind();
    }

    state = ChunkState::BUILT;
}

void Chunk::render() const {
    if (state == ChunkState::BUILT && !vertices.empty()) {
        vao.bind();
        glDrawElements(GL_TRIANGLES, static_cast<int>(vertices.size() / 4) * 6, GL_UNSIGNED_INT, nullptr);
        VertexArray::unbind();
    }
}

void Chunk::renderTransparent() const {
    if (state == ChunkState::BUILT && !transparentVertices.empty()) {
        transparentVAO.bind();
        glDrawElements(GL_TRIANGLES, static_cast<int>(transparentVertices.size() / 4) * 6, GL_UNSIGNED_INT,
                       nullptr);
        VertexArray::unbind();
    }
}

void Chunk::write(vector<char>& data) const {
    for (int x = 0; x < CHUNK_SIZE_X; ++x) {
        for (int y = 0; y < CHUNK_SIZE_Y; ++y) {
            for (int z = 0; z < CHUNK_SIZE_Z; ++z) {
                const auto block = blocks[getIndex(x, y, z)];
                auto id = block.getID();
                auto st = block.getState();

                auto index = (x * CHUNK_SIZE_Z * CHUNK_SIZE_Y + y * CHUNK_SIZE_Z + z) * 3;
                std::memcpy(data.data() + index, &id, sizeof(id));
                std::memcpy(data.data() + index + sizeof(id), &st, sizeof(st));
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
                uint8_t blockState;
                in.read(reinterpret_cast<char*>(&blockState), sizeof(blockState));

                blocks[getIndex(x, y, z)] = Block(id, blockState);
            }
        }
    }

    state = ChunkState::POPULATED;
}

Block Chunk::getBlock(int x, int y, int z) const {
    return blocks[getIndex(x, y, z)];
}

void Chunk::setBlock(int x, int y, int z, const Block& block) {
    blocks[getIndex(x, y, z)] = block;
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

void Chunk::addFace(const BlockType& type, const BlockFace face, const u8vec3& position) {
    for (const auto& vertex : Block::blockFaceVertices[face]) {
        auto v = Vertex(vertex.position + position, vertex.uv, type.getLayer(face));

        if (type.opaque) {
            vertices.push_back(v);
        } else {
            transparentVertices.push_back(v);
        }
    }
}

void Chunk::addBillboard(const BlockType& type, const u8vec3& position) {
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
