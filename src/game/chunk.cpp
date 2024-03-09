#include "chunk.h"

Chunk::Chunk(int x, int z, const shared_ptr<ElementBuffer>& ebo, const shared_ptr<BlockTexture>& blockTexture)
        : chunkPosition(x, 0, z), ebo(ebo), blockTexture(blockTexture) {
    // Fill chunk data with blocks
    for (int bx = 0; bx < CHUNK_SIZE_X; ++bx) {
        for (int by = 0; by < CHUNK_SIZE_Y; ++by) {
            for (int bz = 0; bz < CHUNK_SIZE_Z; ++bz) {
                auto type = by < 65 ? (by < 63 ? BlockType::STONE : BlockType::DIRT) : BlockType::AIR;
                blocks[bz][by][bx] = make_unique<Block>(type);
            }
        }
    }

    // Initialize VertexBuffer attributes
    vao.bind();
    vbo.bind();
    ebo->bind();
    vbo.vertexAttribIPointer(0, 3, GL_UNSIGNED_BYTE, sizeof(Vertex), (void*) nullptr);
    vbo.vertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, uv));
    VertexArray::unbind();
}

Block* Chunk::getBlock(int x, int y, int z) const {
    if (x >= 0 && x < CHUNK_SIZE_X && y >= 0 && y < CHUNK_SIZE_Y && z >= 0 && z < CHUNK_SIZE_Z) {
        return blocks[z][y][x].get();
    }
    return nullptr;
}

void Chunk::buildMesh(Chunk* leftChunk, Chunk* rightChunk, Chunk* backChunk, Chunk* frontChunk) {
    chunkBuilt = false;
    vertices.clear();

    for (int bx = 0; bx < CHUNK_SIZE_X; ++bx) {
        for (int by = 0; by < CHUNK_SIZE_Y; ++by) {
            for (int bz = 0; bz < CHUNK_SIZE_Z; ++bz) {
                auto block = getBlock(bx, by, bz);
                auto localPosition = u8vec3(bx, by, bz);

                if (block != nullptr && block->getType() != BlockType::AIR) {
                    // Left Face
                    Block* leftBlock = nullptr;
                    if (bx > 0) {
                        leftBlock = getBlock(bx - 1, by, bz);
                    } else if (leftChunk != nullptr) {
                        leftBlock = leftChunk->getBlock(CHUNK_SIZE_X - 1, by, bz);
                    }

                    if (leftBlock == nullptr || leftBlock->getType() == BlockType::AIR) {
                        addFace(block, BlockFace::LEFT, localPosition);
                    }

                    // Right Face
                    Block* rightBlock = nullptr;
                    if (bx < CHUNK_SIZE_X - 1) {
                        rightBlock = getBlock(bx + 1, by, bz);
                    } else if (rightChunk != nullptr) {
                        rightBlock = rightChunk->getBlock(0, by, bz);
                    }

                    if (rightBlock == nullptr || rightBlock->getType() == BlockType::AIR) {
                        addFace(block, BlockFace::RIGHT, localPosition);
                    }

                    // Back Face
                    Block* backBlock = nullptr;
                    if (bz > 0) {
                        backBlock = getBlock(bx, by, bz - 1);
                    } else if (backChunk != nullptr) {
                        backBlock = backChunk->getBlock(CHUNK_SIZE_Z - 1, by, bz);
                    }

                    if (backBlock == nullptr || backBlock->getType() == BlockType::AIR) {
                        addFace(block, BlockFace::BACK, localPosition);
                    }

                    // Front Face
                    Block* frontBlock = nullptr;
                    if (bz < CHUNK_SIZE_Z - 1) {
                        frontBlock = getBlock(bx, by, bz + 1);
                    } else if (frontChunk != nullptr) {
                        frontBlock = frontChunk->getBlock(0, by, bz);
                    }

                    if (frontBlock == nullptr || frontBlock->getType() == BlockType::AIR) {
                        addFace(block, BlockFace::FRONT, localPosition);
                    }

                    // Bottom Face
                    Block* bottomBlock = nullptr;
                    if (by > 0) {
                        bottomBlock = getBlock(bx, by - 1, bz);
                    }

                    if (bottomBlock == nullptr || bottomBlock->getType() == BlockType::AIR) {
                        addFace(block, BlockFace::BOTTOM, localPosition);
                    }

                    // Top Face
                    Block* topBlock = nullptr;
                    if (by < CHUNK_SIZE_Y - 1) {
                        topBlock = getBlock(bx, by + 1, bz);
                    }

                    if (topBlock == nullptr || topBlock->getType() == BlockType::AIR) {
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

    chunkBuilt = true;
}

bool Chunk::isChunkBuilt() const {
    return chunkBuilt;
}

const glm::ivec3& Chunk::getChunkPosition() const {
    return chunkPosition;
}

void Chunk::render() {
    vao.bind();
    glDrawElements(GL_TRIANGLES, static_cast<int>(vertices.size() / 4) * 6, GL_UNSIGNED_INT, (void*) nullptr);
    VertexArray::unbind();
}

void Chunk::addFace(Block* block, BlockFace face, const u8vec3& position) {
    Block::addBlockFaceVertices(vertices, face, position);

    if (vertices.size() > 4) {
        auto name = Block::getBlockFaceTexture(block->getType(), face);
        auto coordinates = blockTexture->getTextureCoordinates(name);
        auto offset = blockTexture->getOffset();

        for (auto it = vertices.end() - 4; it != vertices.end(); ++it) {
            it->uv *= offset;
            it->uv += coordinates;
        }
    }
}
