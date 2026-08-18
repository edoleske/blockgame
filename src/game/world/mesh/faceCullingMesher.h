#ifndef BLOCKGAME_FACECULLINGMESHER_H
#define BLOCKGAME_FACECULLINGMESHER_H

#include "mesher.h"


class FaceCullingMesher : public Mesher {
public:
    explicit FaceCullingMesher(int vertexCount, int transparentVertexCount);

    void build(const array<BlockID, (CHUNK_SIZE_X + 2) * (CHUNK_SIZE_Z + 2) * (CHUNK_SIZE_Y + 2)>& buildCache) override;
};


#endif //BLOCKGAME_FACECULLINGMESHER_H
