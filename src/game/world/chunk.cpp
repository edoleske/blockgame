#include "chunk.h"

#include "log.h"
#include "game/world/world.h"
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
    vector<Vertex> vertices;
    vertices.reserve(vertexCount);
    vector<Vertex> transparentVertices;
    transparentVertices.reserve(transparentVertexCount);
    auto dictionary = BlockDictionary::getInstance();

    // Fetch all blocks into buffer for cheaper lookups
    array<BlockID, (CHUNK_SIZE_X + 2) * (CHUNK_SIZE_Z + 2) * (CHUNK_SIZE_Y + 2)> buildCache{};
    for (int bx = 0; bx < CHUNK_SIZE_X + 2; ++bx) {
        for (int by = 0; by < CHUNK_SIZE_Y; ++by) {
            for (int bz = 0; bz < CHUNK_SIZE_Z + 2; ++bz) {
                auto cx = bx > CHUNK_SIZE_X ? 2 : bx > 0 ? 1 : 0;
                auto cz = bz > CHUNK_SIZE_Z ? 2 : bz > 0 ? 1 : 0;
                auto index = getCacheIndex(bx - 1, by, bz - 1);

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

    // Calculate visible faces for each block
    array<uint8_t, CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z> faceCache{};
    for (int bx = 0; bx < CHUNK_SIZE_X; ++bx) {
        for (int by = 0; by < CHUNK_SIZE_Y; ++by) {
            for (int bz = 0; bz < CHUNK_SIZE_Z; ++bz) {
                auto id = buildCache[getCacheIndex(bx, by, bz)];

                // Generate no geometry for air blocks
                if (id == 0) {
                    continue;
                }

                auto block = dictionary->get(id);
                if (block->isBillboard) {
                    addBillboard(vertices, transparentVertices, block, u8vec3(bx, by, bz));
                    continue;
                }

                // Getting adjacent blocks
                uint8_t byte = 0;
                auto left = buildCache[getCacheIndex(bx - 1, by, bz)];
                auto right = buildCache[getCacheIndex(bx + 1, by, bz)];
                auto back = buildCache[getCacheIndex(bx, by, bz - 1)];
                auto front = buildCache[getCacheIndex(bx, by, bz + 1)];
                auto bottom = buildCache[getCacheIndex(bx, by - 1, bz)];
                auto top = buildCache[getCacheIndex(bx, by + 1, bz)];

                if (left != id && isVisibleFace(block, dictionary->get(left))) {
                    byte |= 1 << static_cast<uint8_t>(BlockFace::LEFT);
                }
                if (right != id && isVisibleFace(block, dictionary->get(right))) {
                    byte |= 1 << static_cast<uint8_t>(BlockFace::RIGHT);
                }
                if (back != id && isVisibleFace(block, dictionary->get(back))) {
                    byte |= 1 << static_cast<uint8_t>(BlockFace::BACK);
                }
                if (front != id && isVisibleFace(block, dictionary->get(front))) {
                    byte |= 1 << static_cast<uint8_t>(BlockFace::FRONT);
                }
                if (bottom != id && isVisibleFace(block, dictionary->get(bottom))) {
                    byte |= 1 << static_cast<uint8_t>(BlockFace::BOTTOM);
                }
                if (top != id && isVisibleFace(block, dictionary->get(top))) {
                    byte |= 1 << static_cast<uint8_t>(BlockFace::TOP);
                }

                faceCache[getIndex(bx, by, bz)] = byte;
            }
        }
    }

    // XZ Plane
    // Future function parameters
    // a = Axis (0, 1, 2) for (x, y, z)
    // auto a = 1;
    // for (int by = 0; by < CHUNK_SIZE_Y; ++by) {
    //     array<bool, CHUNK_SIZE_X * CHUNK_SIZE_Z> visited{};
    //
    //     for (int bx = 0; bx < CHUNK_SIZE_X; ++bx) {
    //         for (int bz = 0; bz < CHUNK_SIZE_Z; ++bz) {
    //             auto faces = faceCache[getIndex(bx, by, bz)];
    //             if (!(faces << static_cast<uint8_t>(BlockFace::TOP) & 0x1)) continue;
    //
    //             if (visited[bx * CHUNK_SIZE_Z + bz]) continue;
    //
    //             auto id = buildCache[getCacheIndex(bx, by, bz)];
    //
    //             int ex = bx, ez = bz;
    //             for (int i = ex; i < CHUNK_SIZE_X; ++i) {
    //                 int fz = bz;
    //                 for (int j = i > bx ? bz : bz + 1; j < CHUNK_SIZE_Z && (i == bx || j <= ez); ++j) {
    //                     if (!(faceCache[getIndex(i, by, j)] << static_cast<uint8_t>(BlockFace::TOP) & 0x1)) {
    //                         break;
    //                     }
    //
    //                     auto next = buildCache[getCacheIndex(i, by, j)];
    //                     if (id != next) {
    //                         break;
    //                     }
    //
    //                     if (visited[bx * CHUNK_SIZE_Z + bz]) break;
    //
    //                     fz = j;
    //                 }
    //                 if (i > bx && fz < ez) break;
    //                 ez = fz;
    //
    //                 for (int k = bz; k <= fz; ++k) {
    //                     visited[i * CHUNK_SIZE_Z + k] = true;
    //                 }
    //                 ex = i;
    //
    //                 if (i == bx && fz == bz) break;
    //             }
    //
    //             addFace(vertices, transparentVertices, dictionary->get(id), BlockFace::TOP, u8vec3(bx, by, bz),
    //                     u8vec3(ex, by, ez));
    //         }
    //     }
    // }
    greedyMesh(vertices, transparentVertices, buildCache, faceCache, 0);
    greedyMesh(vertices, transparentVertices, buildCache, faceCache, 1);
    greedyMesh(vertices, transparentVertices, buildCache, faceCache, 2);

    // for (int bx = 0; bx < CHUNK_SIZE_X; ++bx) {
    //     for (int by = 0; by < CHUNK_SIZE_Y; ++by) {
    //         for (int bz = 0; bz < CHUNK_SIZE_Z; ++bz) {
    //             auto id = buildCache[getCacheIndex(bx, by, bz)];
    //
    //             // Generate no geometry for air blocks
    //             if (id == 0) {
    //                 continue;
    //             }
    //
    //             auto block = dictionary->get(id);
    //             auto localPosition = u8vec3(bx, by, bz);
    //
    //             // Getting adjacent blocks
    //             auto left = buildCache[getCacheIndex(bx - 1, by, bz)];
    //             auto right = buildCache[getCacheIndex(bx + 1, by, bz)];
    //             auto back = buildCache[getCacheIndex(bx, by, bz - 1)];
    //             auto front = buildCache[getCacheIndex(bx, by, bz + 1)];
    //             auto bottom = buildCache[getCacheIndex(bx, by - 1, bz)];
    //             auto top = buildCache[getCacheIndex(bx, by + 1, bz)];
    //
    //             if (left != id && isVisibleFace(block, dictionary->get(left))) {
    //                 addFace(vertices, transparentVertices, block, BlockFace::LEFT, localPosition);
    //             }
    //             if (right != id && isVisibleFace(block, dictionary->get(right))) {
    //                 addFace(vertices, transparentVertices, block, BlockFace::RIGHT, localPosition);
    //             }
    //             if (back != id && isVisibleFace(block, dictionary->get(back))) {
    //                 addFace(vertices, transparentVertices, block, BlockFace::BACK, localPosition);
    //             }
    //             if (front != id && isVisibleFace(block, dictionary->get(front))) {
    //                 addFace(vertices, transparentVertices, block, BlockFace::FRONT, localPosition);
    //             }
    //             if (bottom != id && isVisibleFace(block, dictionary->get(bottom))) {
    //                 addFace(vertices, transparentVertices, block, BlockFace::BOTTOM, localPosition);
    //             }
    //             if (top != id && isVisibleFace(block, dictionary->get(top))) {
    //                 addFace(vertices, transparentVertices, block, BlockFace::TOP, localPosition);
    //             }
    //         }
    //     }
    // }

    vertices.shrink_to_fit();
    vertexCount = static_cast<int>(vertices.size());
    transparentVertices.shrink_to_fit();
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

int Chunk::getVertexCount() const {
    return vertexCount + transparentVertexCount;
}

bool Chunk::isValidBlockPosition(const int x, const int y, const int z) {
    return x >= 0 && x < CHUNK_SIZE_X &&
        y >= 0 && y < CHUNK_SIZE_Y &&
        z >= 0 && z < CHUNK_SIZE_Z;
}

void Chunk::greedyMesh(
    vector<Vertex>& vertices, vector<Vertex>& transparentVertices,
    const array<BlockID, (CHUNK_SIZE_X + 2) * (CHUNK_SIZE_Z + 2) * (CHUNK_SIZE_Y + 2)>& buildCache,
    const array<uint8_t, CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z>& faceCache,
    const int axis) {
    auto aSize = axis == 1 ? CHUNK_SIZE_Y : CHUNK_SIZE_X;
    auto uSize = axis == 1 ? CHUNK_SIZE_X : CHUNK_SIZE_Y;
    auto vSize = CHUNK_SIZE_X;

    auto posFace = BlockFace::RIGHT;
    auto negFace = BlockFace::LEFT;
    if (axis == 2) {
        posFace = BlockFace::FRONT;
        negFace = BlockFace::BACK;
    } else if (axis == 1) {
        posFace = BlockFace::TOP;
        negFace = BlockFace::BOTTOM;
    }

    auto dictionary = BlockDictionary::getInstance();
    vector<bool> posVisited(uSize * vSize);
    vector<bool> negVisited(uSize * vSize);

    for (int a = 0; a < aSize; ++a) {
        posVisited.assign(uSize * vSize, false);
        negVisited.assign(uSize * vSize, false);

        for (int u = 0; u < uSize; ++u) {
            for (int v = 0; v < vSize; ++v) {
                auto pos = swizzle(axis, a, u, v);
                auto bx = pos.x;
                auto by = pos.y;
                auto bz = pos.z;

                auto id = buildCache[getCacheIndex(bx, by, bz)];
                auto faces = faceCache[getIndex(bx, by, bz)];

                if ((faces >> static_cast<uint8_t>(posFace)) & 0x1 && !posVisited[u * vSize + v]) {
                    auto end = getFaceSize(a, u, v, posFace, uSize, vSize, id, axis, buildCache, faceCache, posVisited);
                    addFace(vertices, transparentVertices, dictionary->get(id), posFace, u8vec3(bx, by, bz), end);
                }

                if ((faces >> static_cast<uint8_t>(negFace)) & 0x1 && !negVisited[u * vSize + v]) {
                    auto end = getFaceSize(a, u, v, negFace, uSize, vSize, id, axis, buildCache, faceCache, negVisited);
                    addFace(vertices, transparentVertices, dictionary->get(id), negFace, u8vec3(bx, by, bz), end);
                }
            }
        }
    }
}

u8vec3 Chunk::getFaceSize(
    const int a, const int u, const int v, BlockFace face, const int uSize, const int vSize, const BlockID block,
    const int axis,
    const array<BlockID, (CHUNK_SIZE_X + 2) * (CHUNK_SIZE_Z + 2) * (CHUNK_SIZE_Y + 2)>& buildCache,
    const array<uint8_t, CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z>& faceCache, vector<bool>& visited) {

    int eu = u, ev = v;
    for (int i = eu; i < uSize; ++i) {
        int fv = i > u ? v - 1 : v;
        for (int j = i > u ? v : v + 1; j < vSize && (i == u || j <= ev); ++j) {
            auto p = swizzle(axis, a, i, j);

            if (!((faceCache[getIndex(p.x, p.y, p.z)] >> static_cast<uint8_t>(face)) & 0x1)) {
                break;
            }

            auto next = buildCache[getCacheIndex(p.x, p.y, p.z)];
            if (block != next) {
                break;
            }

            if (visited[i * vSize + j]) {
                break;
            }

            fv = j;
        }
        if (i > u && fv < ev) break;
        if (i == u) ev = fv;

        for (int k = v; k <= fv; ++k) {
            visited[i * vSize + k] = true;
        }
        eu = i;
    }

    return swizzle(axis, a, eu, ev);
}

ivec3 Chunk::swizzle(const int axis, int a, int u, int v) {
    switch (axis) {
    case 2:
        return {v, u, a};
    case 1:
        return {u, a, v};
    case 0:
    default:
        return {a, u, v};
    }
}

void Chunk::addFace(
    vector<Vertex>& vertices, vector<Vertex>& transparentVertices, const Block* block, const BlockFace face,
    const u8vec3& position) {
    for (const auto& vertex : Block::blockFaceVertices[face]) {
        auto v = Vertex(vertex.position + position, vertex.uv, block->getLayer(face));

        if (block->opaque) {
            vertices.push_back(v);
        } else {
            transparentVertices.push_back(v);
        }
    }
}

void Chunk::addFace(
    vector<Vertex>& vertices, vector<Vertex>& transparentVertices, const Block* block, const BlockFace face,
    const u8vec3& origin, const u8vec3& end) {
    if (origin == end) {
        addFace(vertices, transparentVertices, block, face, origin);
        return;
    }

    auto difference = end - origin;

    for (const auto& vertex : Block::blockFaceVertices[face]) {
        auto uv = vertex.uv;
        switch (face) {
        case BlockFace::FRONT:
        case BlockFace::BACK:
            uv += uv * u8vec2(difference.x, difference.y);
            break;
        case BlockFace::LEFT:
        case BlockFace::RIGHT:
            uv += uv * u8vec2(difference.z, difference.y);
            break;
        case BlockFace::TOP:
        case BlockFace::BOTTOM:
            uv += uv * u8vec2(difference.x, difference.z);
            break;
        }

        auto v = Vertex((vertex.position * difference + vertex.position) + origin, uv, block->getLayer(face));

        if (block->opaque) {
            vertices.push_back(v);
        } else {
            transparentVertices.push_back(v);
        }
    }
}

void Chunk::addBillboard(
    vector<Vertex>& vertices, vector<Vertex>& transparentVertices, const Block* block, const u8vec3& position) {
    for (const auto& vertex : Block::billboardVertices) {
        auto v = Vertex(vertex.position + position, vertex.uv, block->getLayer(BlockFace::FRONT));

        if (block->opaque) {
            vertices.push_back(v);
        } else {
            transparentVertices.push_back(v);
        }
    }
}

void Chunk::setChunkState(const ChunkState newState) {
    state = newState;
}

int Chunk::getIndex(const int x, const int y, const int z) {
    return x * CHUNK_SIZE_Y * CHUNK_SIZE_Z + y * CHUNK_SIZE_Z + z;
}

int Chunk::getCacheIndex(const int x, const int y, const int z) {
    return (x + 1) * (CHUNK_SIZE_Y + 2) * (CHUNK_SIZE_Z + 2) + (y + 1) * (CHUNK_SIZE_Z + 2) + (z + 1);
}

bool Chunk::isVisibleFace(const Block* a, const Block* b) {
    return a->opaque != b->opaque || (!a->opaque && !b->opaque && a->id != b->id);
}
