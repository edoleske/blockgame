#include "chunk.h"

Chunk::Chunk(int x, int z, const shared_ptr<ElementBuffer>& ebo, const shared_ptr<BlockTexture>& blockTexture)
        : chunkPosition(x, 0, z), ebo(ebo), blockTexture(blockTexture) {
    // Initialize VertexBuffer attributes
    vao.bind();
    vbo.bind();
    ebo->bind();
    vbo.vertexAttribIPointer(0, 3, GL_UNSIGNED_BYTE, sizeof(Vertex), (void*) nullptr);
    vbo.vertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, uv));
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

    for (int bx = 0; bx < CHUNK_SIZE_X; ++bx) {
        for (int by = 0; by < CHUNK_SIZE_Y; ++by) {
            for (int bz = 0; bz < CHUNK_SIZE_Z; ++bz) {
                Block block = blocks[getIndex(bx, by, bz)];
                auto localPosition = u8vec3(bx, by, bz);

                if (block.getType() != BlockType::AIR) {
                    // Left Face
                    bool leftFace = bx > 0 ?
                                    getBlock(bx - 1, by, bz).getType() == BlockType::AIR :
                                    leftChunk->getBlock(CHUNK_SIZE_X - 1, by, bz).getType() == BlockType::AIR;

                    if (leftFace) {
                        addFace(block, BlockFace::LEFT, localPosition);
                    }

                    // Right Face
                    bool rightFace = bx < CHUNK_SIZE_X - 1 ?
                                     getBlock(bx + 1, by, bz).getType() == BlockType::AIR :
                                     rightChunk->getBlock(0, by, bz).getType() == BlockType::AIR;

                    if (rightFace) {
                        addFace(block, BlockFace::RIGHT, localPosition);
                    }

                    // Back Face
                    bool backFace = bz > 0 ?
                                    getBlock(bx, by, bz - 1).getType() == BlockType::AIR :
                                    backChunk->getBlock(bx, by, CHUNK_SIZE_Z - 1).getType() == BlockType::AIR;

                    if (backFace) {
                        addFace(block, BlockFace::BACK, localPosition);
                    }

                    // Front Face
                    bool frontFace = bz < CHUNK_SIZE_Z - 1 ?
                                     getBlock(bx, by, bz + 1).getType() == BlockType::AIR :
                                     frontChunk->getBlock(bx, by, 0).getType() == BlockType::AIR;

                    if (frontFace) {
                        addFace(block, BlockFace::FRONT, localPosition);
                    }

                    // Bottom Face
                    bool bottomFace = by > 0 && getBlock(bx, by - 1, bz).getType() == BlockType::AIR;

                    if (bottomFace) {
                        addFace(block, BlockFace::BOTTOM, localPosition);
                    }

                    // Top Face
                    bool topFace = by < CHUNK_SIZE_Y - 1 && getBlock(bx, by + 1, bz).getType() == BlockType::AIR;

                    if (topFace) {
                        addFace(block, BlockFace::TOP, localPosition);
                    }
                }
            }
        }
    }

    // Copy vertices to VBO
    vao.bind();
    vbo.bind();
    vbo.bufferData(vertices.size() * sizeof(Vertex), &vertices.front(), GL_STATIC_DRAW);
    VertexArray::unbind();

    state = ChunkState::BUILT;
}

void Chunk::generate(const NoiseGenerator* noise) {
    // 2D height map
    auto heightMap = std::array<std::array<int, CHUNK_SIZE_Z>, CHUNK_SIZE_X>();
    for (int bx = 0; bx < CHUNK_SIZE_X; ++bx) {
        auto fx = static_cast<float>(chunkPosition.x) + (static_cast<float>(bx + 1) / CHUNK_SIZE_X) +
                  1.0f / (2 * CHUNK_SIZE_X);
        for (int bz = 0; bz < CHUNK_SIZE_Z; ++bz) {
            auto fz = static_cast<float>(chunkPosition.z) + (static_cast<float>(bz + 1) / CHUNK_SIZE_Z) +
                      1.0f / (2 * CHUNK_SIZE_X);
            heightMap[bx][bz] = 50 + static_cast<int>(noise->get(fx * 0.05f, 0.05f, fz * 0.05f) * 50);
        }
    }

    // Fill chunk data with blocks
    for (int bx = 0; bx < CHUNK_SIZE_X; ++bx) {
        for (int bz = 0; bz < CHUNK_SIZE_Z; ++bz) {
            auto height = heightMap[bx][bz];
            for (int by = 0; by < CHUNK_SIZE_Y; ++by) {
                auto type = BlockType::AIR;

                if (by == height) {
                    type = BlockType::GRASS;
                } else if (by < height && by > height - 3) {
                    type = BlockType::DIRT;
                } else if (by > 0 && by < height - 2) {
                    type = BlockType::STONE;
                } else if (by == 0) {
                    type = BlockType::BEDROCK;
                }

                blocks[getIndex(bx, by, bz)] = Block(type);
            }
        }
    }

    state = ChunkState::POPULATED;
}

void Chunk::render() {
    if (state == ChunkState::BUILT) {
        vao.bind();
        glDrawElements(GL_TRIANGLES, static_cast<int>(vertices.size() / 4) * 6, GL_UNSIGNED_INT, (void*) nullptr);
        VertexArray::unbind();
    }
}

void Chunk::write(vector<char>& data) {
    for (int x = 0; x < CHUNK_SIZE_X; ++x) {
        for (int y = 0; y < CHUNK_SIZE_Y; ++y) {
            for (int z = 0; z < CHUNK_SIZE_Z; ++z) {
                auto type = static_cast<uint16_t>(blocks[getIndex(x, y, z)].getType());
                uint8_t blockState = blocks[getIndex(x, y, z)].getState();

                auto index = (x * CHUNK_SIZE_Z * CHUNK_SIZE_Y + y * CHUNK_SIZE_Z + z) * 3;
                data[index] = static_cast<const char>(type);
                data[index + 1] = static_cast<const char>(type >> 8);
                data[index + 2] = static_cast<const char>(blockState);
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
                uint16_t type;
                in.read((char*) &type, sizeof(type));
                uint8_t blockState;
                in.read((char*) &blockState, sizeof(blockState));

                blocks[getIndex(x, y, z)] = Block(static_cast<BlockType>(type), blockState);
            }
        }
    }

    state = ChunkState::POPULATED;
}

Block Chunk::getBlock(int x, int y, int z) const {
    return blocks[getIndex(x, y, z)];
}

ChunkState Chunk::getChunkState() const {
    return state;
}

const glm::ivec3& Chunk::getChunkPosition() const {
    return chunkPosition;
}

void Chunk::addFace(const Block& block, BlockFace face, const u8vec3& position) {
    auto name = Block::getBlockFaceTexture(block.getType(), face);
    auto coordinates = blockTexture->getTextureCoordinates(name);
    auto offset = blockTexture->getOffset();

    for (const auto& vertex: Block::blockFaceVertices[face]) {
        vertices.push_back(Vertex(vertex.position + position, (vertex.uv * offset) + coordinates));
    }
}

void Chunk::setChunkState(ChunkState state) {
    Chunk::state = state;
}

int Chunk::getIndex(int x, int y, int z) {
    return x * CHUNK_SIZE_Y * CHUNK_SIZE_Z + y * CHUNK_SIZE_Z + z;
}
