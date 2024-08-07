#include "chunk.h"

Chunk::Chunk(int x, int z, const shared_ptr<ElementBuffer>& ebo, const shared_ptr<BlockTexture>& blockTexture)
        : chunkPosition(x, 0, z), ebo(ebo), blockTexture(blockTexture) {
    // Initialize VertexBuffer attributes
    vao.bind();
    vbo.bind();
    ebo->bind();
    vbo.vertexAttribIPointer(0, 3, GL_UNSIGNED_BYTE, sizeof(Vertex), (void*) nullptr);
    vbo.vertexAttribIPointer(1, 2, GL_UNSIGNED_SHORT, sizeof(Vertex), (void*) offsetof(Vertex, uv));
    VertexArray::unbind();

    transparentVAO.bind();
    transparentVBO.bind();
    ebo->bind();
    transparentVBO.vertexAttribIPointer(0, 3, GL_UNSIGNED_BYTE, sizeof(Vertex), (void*) nullptr);
    transparentVBO.vertexAttribIPointer(1, 2, GL_UNSIGNED_SHORT, sizeof(Vertex), (void*) offsetof(Vertex, uv));
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
                Block block = blocks[getIndex(bx, by, bz)];
                auto type = block.getType();
                if (type == BlockType::AIR) {
                    continue;
                }

                auto localPosition = u8vec3(bx, by, bz);

                if (!block.isOpaque()) {
                    // Check if transparent block is a billboard
                    if (Block::isBlockTypeBillboard(type)) {
                        addBillboard(block, localPosition);
                        continue;
                    }

                    // Left Face
                    bool leftFace = bx > 0 ?
                                    getBlock(bx - 1, by, bz).isDifferentTransparent(type) :
                                    leftChunk->getBlock(CHUNK_SIZE_X - 1, by, bz).isDifferentTransparent(type);

                    if (leftFace) {
                        addFace(block, BlockFace::LEFT, localPosition, true);
                    }

                    // Right Face
                    bool rightFace = bx < CHUNK_SIZE_X - 1 ?
                                     getBlock(bx + 1, by, bz).isDifferentTransparent(type) :
                                     rightChunk->getBlock(0, by, bz).isDifferentTransparent(type);

                    if (rightFace) {
                        addFace(block, BlockFace::RIGHT, localPosition, true);
                    }

                    // Back Face
                    bool backFace = bz > 0 ?
                                    getBlock(bx, by, bz - 1).isDifferentTransparent(type) :
                                    backChunk->getBlock(bx, by, CHUNK_SIZE_Z - 1).isDifferentTransparent(type);

                    if (backFace) {
                        addFace(block, BlockFace::BACK, localPosition, true);
                    }

                    // Front Face
                    bool frontFace = bz < CHUNK_SIZE_Z - 1 ?
                                     getBlock(bx, by, bz + 1).isDifferentTransparent(type) :
                                     frontChunk->getBlock(bx, by, 0).isDifferentTransparent(type);

                    if (frontFace) {
                        addFace(block, BlockFace::FRONT, localPosition, true);
                    }

                    // Bottom Face
                    bool bottomFace = by > 0 && getBlock(bx, by - 1, bz).isDifferentTransparent(type);

                    if (bottomFace) {
                        addFace(block, BlockFace::BOTTOM, localPosition, true);
                    }

                    // Top Face
                    bool topFace = by < CHUNK_SIZE_Y - 1 && getBlock(bx, by + 1, bz).isDifferentTransparent(type);

                    if (topFace) {
                        addFace(block, BlockFace::TOP, localPosition, true);
                    }
                } else {
                    // Left Face
                    bool leftFace = bx > 0 ?
                                    !getBlock(bx - 1, by, bz).isOpaque() :
                                    !leftChunk->getBlock(CHUNK_SIZE_X - 1, by, bz).isOpaque();

                    if (leftFace) {
                        addFace(block, BlockFace::LEFT, localPosition);
                    }

                    // Right Face
                    bool rightFace = bx < CHUNK_SIZE_X - 1 ?
                                     !getBlock(bx + 1, by, bz).isOpaque() :
                                     !rightChunk->getBlock(0, by, bz).isOpaque();

                    if (rightFace) {
                        addFace(block, BlockFace::RIGHT, localPosition);
                    }

                    // Back Face
                    bool backFace = bz > 0 ?
                                    !getBlock(bx, by, bz - 1).isOpaque() :
                                    !backChunk->getBlock(bx, by, CHUNK_SIZE_Z - 1).isOpaque();

                    if (backFace) {
                        addFace(block, BlockFace::BACK, localPosition);
                    }

                    // Front Face
                    bool frontFace = bz < CHUNK_SIZE_Z - 1 ?
                                     !getBlock(bx, by, bz + 1).isOpaque() :
                                     !frontChunk->getBlock(bx, by, 0).isOpaque();

                    if (frontFace) {
                        addFace(block, BlockFace::FRONT, localPosition);
                    }

                    // Bottom Face
                    bool bottomFace = by > 0 && !getBlock(bx, by - 1, bz).isOpaque();

                    if (bottomFace) {
                        addFace(block, BlockFace::BOTTOM, localPosition);
                    }

                    // Top Face
                    bool topFace = by < CHUNK_SIZE_Y - 1 && !getBlock(bx, by + 1, bz).isOpaque();

                    if (topFace) {
                        addFace(block, BlockFace::TOP, localPosition);
                    }
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

void Chunk::render() {
    if (state == ChunkState::BUILT && !vertices.empty()) {
        vao.bind();
        glDrawElements(GL_TRIANGLES, static_cast<int>(vertices.size() / 4) * 6, GL_UNSIGNED_INT, (void*) nullptr);
        VertexArray::unbind();
    }
}

void Chunk::renderTransparent() {
    if (state == ChunkState::BUILT && !transparentVertices.empty()) {
        transparentVAO.bind();
        glDrawElements(GL_TRIANGLES, static_cast<int>(transparentVertices.size() / 4) * 6, GL_UNSIGNED_INT,
                       (void*) nullptr);
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

void Chunk::setBlock(int x, int y, int z, const Block& block) {
    blocks[getIndex(x, y, z)] = block;
}

ChunkState Chunk::getChunkState() const {
    return state;
}

const glm::ivec3& Chunk::getChunkPosition() const {
    return chunkPosition;
}

bool Chunk::isValidBlockPosition(int x, int y, int z) {
    return x >= 0 && x < CHUNK_SIZE_X &&
           y >= 0 && y < CHUNK_SIZE_Y &&
           z >= 0 && z < CHUNK_SIZE_Z;
}

void Chunk::addFace(const Block& block, BlockFace face, const u8vec3& position, bool transparent) {
    auto name = Block::getBlockFaceTexture(block.getType(), face);
    auto coordinates = blockTexture->getTextureCoordinates(name);

    for (const auto& vertex: Block::blockFaceVertices[face]) {
        auto v = Vertex(vertex.position + position, (vertex.uv * BlockTexture::RESOLUTION) + coordinates);

        if (transparent) {
            transparentVertices.push_back(v);
        } else {
            vertices.push_back(v);
        }
    }
}

void Chunk::addBillboard(const Block& block, const u8vec3& position) {
    auto name = Block::getBlockFaceTexture(block.getType(), BlockFace::LEFT);
    auto coordinates = blockTexture->getTextureCoordinates(name);


    for (const auto& vertex: Block::billboardVertices) {
        transparentVertices.emplace_back(vertex.position + position,
                                         (vertex.uv * BlockTexture::RESOLUTION) + coordinates);
    }
}

void Chunk::setChunkState(ChunkState state) {
    Chunk::state = state;
}

int Chunk::getIndex(int x, int y, int z) {
    return x * CHUNK_SIZE_Y * CHUNK_SIZE_Z + y * CHUNK_SIZE_Z + z;
}
