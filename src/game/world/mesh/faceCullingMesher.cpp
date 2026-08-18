#include "faceCullingMesher.h"

#include "game/block/blockDictionary.h"

FaceCullingMesher::FaceCullingMesher(const int vertexCount, const int transparentVertexCount) : Mesher(
    vertexCount, transparentVertexCount) {}

void FaceCullingMesher::build(
    const array<BlockID, (CHUNK_SIZE_X + 2) * (CHUNK_SIZE_Z + 2) * (CHUNK_SIZE_Y + 2)>& buildCache) {
    auto dictionary = BlockDictionary::getInstance();

    for (int bx = 0; bx < CHUNK_SIZE_X; ++bx) {
        for (int by = 0; by < CHUNK_SIZE_Y; ++by) {
            for (int bz = 0; bz < CHUNK_SIZE_Z; ++bz) {
                auto id = buildCache[getCacheIndex(bx, by, bz)];

                // Generate no geometry for air blocks
                if (id == 0) {
                    continue;
                }

                auto block = dictionary->get(id);
                auto localPosition = u8vec3(bx, by, bz);
                if (block->isBillboard) {
                    addBillboard(block, localPosition);
                    continue;
                }

                // Getting adjacent blocks
                auto left = buildCache[getCacheIndex(bx - 1, by, bz)];
                auto right = buildCache[getCacheIndex(bx + 1, by, bz)];
                auto back = buildCache[getCacheIndex(bx, by, bz - 1)];
                auto front = buildCache[getCacheIndex(bx, by, bz + 1)];
                auto bottom = buildCache[getCacheIndex(bx, by - 1, bz)];
                auto top = buildCache[getCacheIndex(bx, by + 1, bz)];

                if (left != id && isVisibleFace(block, dictionary->get(left))) {
                    addFace(block, BlockFace::LEFT, localPosition);
                }
                if (right != id && isVisibleFace(block, dictionary->get(right))) {
                    addFace(block, BlockFace::RIGHT, localPosition);
                }
                if (back != id && isVisibleFace(block, dictionary->get(back))) {
                    addFace(block, BlockFace::BACK, localPosition);
                }
                if (front != id && isVisibleFace(block, dictionary->get(front))) {
                    addFace(block, BlockFace::FRONT, localPosition);
                }
                if (bottom != id && isVisibleFace(block, dictionary->get(bottom))) {
                    addFace(block, BlockFace::BOTTOM, localPosition);
                }
                if (top != id && isVisibleFace(block, dictionary->get(top))) {
                    addFace(block, BlockFace::TOP, localPosition);
                }
            }
        }
    }

    vertices.shrink_to_fit();
    transparentVertices.shrink_to_fit();
}
