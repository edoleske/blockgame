#include "greedyMesher.h"

#include "game/block/blockDictionary.h"

GreedyMesher::GreedyMesher(const int vertexCount, const int transparentVertexCount)
    : Mesher(vertexCount, transparentVertexCount) {}

void GreedyMesher::build(
    const array<BlockID, (CHUNK_SIZE_X + 2) * (CHUNK_SIZE_Z + 2) * (CHUNK_SIZE_Y + 2)>& buildCache) {
    auto dictionary = BlockDictionary::getInstance();

    // Calculate visible faces for each block
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
                    addBillboard(block, u8vec3(bx, by, bz));
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

    greedyMesh(0, buildCache);
    greedyMesh(1, buildCache);
    greedyMesh(2, buildCache);

    vertices.shrink_to_fit();
    transparentVertices.shrink_to_fit();
}


void GreedyMesher::greedyMesh(
    const int axis,
    const array<BlockID, (CHUNK_SIZE_X + 2) * (CHUNK_SIZE_Z + 2) * (CHUNK_SIZE_Y + 2)>& buildCache) {
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
                    auto end = getFaceSize(a, u, v, posFace, uSize, vSize, id, axis, buildCache, posVisited);
                    addFace(dictionary->get(id), posFace, u8vec3(bx, by, bz), end);
                }

                if ((faces >> static_cast<uint8_t>(negFace)) & 0x1 && !negVisited[u * vSize + v]) {
                    auto end = getFaceSize(a, u, v, negFace, uSize, vSize, id, axis, buildCache, negVisited);
                    addFace(dictionary->get(id), negFace, u8vec3(bx, by, bz), end);
                }
            }
        }
    }
}

u8vec3 GreedyMesher::getFaceSize(
    const int a, const int u, const int v, BlockFace face, const int uSize, const int vSize, const BlockID block,
    const int axis,
    const array<BlockID, (CHUNK_SIZE_X + 2) * (CHUNK_SIZE_Z + 2) * (CHUNK_SIZE_Y + 2)>& buildCache,
    vector<bool>& visited) const {
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
