#ifndef BLOCKGAME_GREEDYMESHER_H
#define BLOCKGAME_GREEDYMESHER_H

#include "common.h"
#include "mesher.h"


// Dramatically reduces vertex count, but makes lighting harder...
class GreedyMesher : public Mesher {
public:
    explicit GreedyMesher(int vertexCount, int transparentVertexCount);

    void build(
        const array<BlockID, (CHUNK_SIZE_X + 2) * (CHUNK_SIZE_Z + 2) * (CHUNK_SIZE_Y + 2)>& buildCache) override;

private:
    array<uint8_t, CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z> faceCache{};

    void greedyMesh(
        int axis,
        const array<BlockID, (CHUNK_SIZE_X + 2) * (CHUNK_SIZE_Z + 2) * (CHUNK_SIZE_Y + 2)>& buildCache);

    u8vec3 getFaceSize(
        int a, int u, int v, BlockFace face, int uSize, int vSize, BlockID block, int axis,
        const array<BlockID, (CHUNK_SIZE_X + 2) * (CHUNK_SIZE_Z + 2) * (CHUNK_SIZE_Y + 2)>& buildCache,
        vector<bool>& visited) const;
};


#endif //BLOCKGAME_GREEDYMESHER_H
