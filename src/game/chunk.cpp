#include "chunk.h"

Chunk::Chunk(int x, int z): chunkPosition(x, 0, z) {
    // Fill chunk data with blocks
    for (int bx = 0; bx < CHUNK_SIZE_X; ++bx) {
        for (int by = 0; by < CHUNK_SIZE_Y; ++by) {
            for (int bz = 0; bz < CHUNK_SIZE_Z; ++bz) {
                blocks[bz][by][bx] = make_unique<Block>(by < 65 ? BlockType::STONE : BlockType::AIR);
            }
        }
    }

    // Initialize VertexBuffer attributes
    vao.bind();
    vbo.bind();
    vbo.vertexAttribIPointer(0, 3, GL_UNSIGNED_BYTE, sizeof(Vertex), (void*)nullptr);
    vbo.vertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
    VertexArray::unbind();
}

Block *Chunk::getBlock(int x, int y, int z) const {
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
                    if (bx == 0) {
                        if (leftChunk != nullptr) {
                            leftBlock = leftChunk->getBlock(CHUNK_SIZE_X - 1, by, bz);
                        }
                    } else {
                        leftBlock = getBlock(bx - 1, by, bz);
                    }

                    if (leftBlock == nullptr || leftBlock->getType() == BlockType::AIR) {
                        Block::addBlockFaceVertices(vertices, BlockFace::LEFT, localPosition);
                    }

                    // Right Face
                    Block* rightBlock = nullptr;
                    if (bx == CHUNK_SIZE_X - 1) {
                        if (rightChunk != nullptr) {
                            rightBlock = rightChunk->getBlock(0, by, bz);
                        }
                    } else {
                        rightBlock = getBlock(bx + 1, by, bz);
                    }

                    if (rightBlock == nullptr || rightBlock->getType() == BlockType::AIR) {
                        Block::addBlockFaceVertices(vertices, BlockFace::RIGHT, localPosition);
                    }

                    // Back Face
                    Block* backBlock = nullptr;
                    if (bz == 0) {
                        if (backChunk != nullptr) {
                            backBlock = backChunk->getBlock(CHUNK_SIZE_Z - 1, by, bz);
                        }
                    } else {
                        backBlock = getBlock(bx, by, bz - 1);
                    }

                    if (backBlock == nullptr || backBlock->getType() == BlockType::AIR) {
                        Block::addBlockFaceVertices(vertices, BlockFace::BACK, localPosition);
                    }

                    // Front Face
                    Block* frontBlock = nullptr;
                    if (bz == CHUNK_SIZE_Z - 1) {
                        if (frontChunk != nullptr) {
                            frontBlock = frontChunk->getBlock(0, by, bz);
                        }
                    } else {
                        frontBlock = getBlock(bx, by, bz + 1);
                    }

                    if (frontBlock == nullptr || frontBlock->getType() == BlockType::AIR) {
                        Block::addBlockFaceVertices(vertices, BlockFace::FRONT, localPosition);
                    }

                    // Bottom Face
                    Block* bottomBlock = nullptr;
                    if (by > 0) {
                        bottomBlock = getBlock(bx, by - 1, bz);
                    }

                    if (bottomBlock == nullptr || bottomBlock->getType() == BlockType::AIR) {
                        Block::addBlockFaceVertices(vertices, BlockFace::BOTTOM, localPosition);
                    }

                    // Top Face
                    Block* topBlock = nullptr;
                    if (by < CHUNK_SIZE_Y - 1) {
                        topBlock = getBlock(bx, by + 1, bz);
                    }

                    if (topBlock == nullptr || topBlock->getType() == BlockType::AIR) {
                        Block::addBlockFaceVertices(vertices, BlockFace::TOP, localPosition);
                    }
                }
            }
        }
    }

    for (auto &vertex: vertices) {
        vertex.uv *= 0.25f;
        vertex.uv.x += 0.25f;
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

const glm::ivec3 &Chunk::getChunkPosition() const {
    return chunkPosition;
}

void Chunk::render() {
    vao.bind();
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    VertexArray::unbind();
}
