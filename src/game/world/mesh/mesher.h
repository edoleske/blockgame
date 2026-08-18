#ifndef BLOCKGAME_MESHER_H
#define BLOCKGAME_MESHER_H

#include "common.h"
#include "game/block/block.h"


// Abstract class for building chunk mesh
class Mesher {
public:
    explicit Mesher(int vertexCount, int transparentVertexCount);

    virtual ~Mesher() = default;

    virtual void build(
        const array<BlockID, (CHUNK_SIZE_X + 2) * (CHUNK_SIZE_Z + 2) * (CHUNK_SIZE_Y + 2)>& buildCache) = 0;

    const vector<Vertex>& getVertices() const;

    const vector<Vertex>& getTransparentVertices() const;

    // Helper to get expanded block cache index from local coordinates (includes -1, 16 indices)
    static int getCacheIndex(const int x, const int y, const int z) {
        return (x + 1) * (CHUNK_SIZE_Y + 2) * (CHUNK_SIZE_Z + 2) + (y + 1) * (CHUNK_SIZE_Z + 2) + (z + 1);
    }

protected:
    vector<Vertex> vertices;
    vector<Vertex> transparentVertices;

    static ivec3 swizzle(const int axis, int a, int u, int v) {
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

    void addFace(const Block* block, BlockFace face, const u8vec3& position);

    void addFace(const Block* block, BlockFace face, const u8vec3& origin, const u8vec3& end);

    void addBillboard(const Block* block, const u8vec3& position);

    static bool isVisibleFace(const Block* a, const Block* b) {
        return a->opaque != b->opaque || (!a->opaque && !b->opaque && a->id != b->id);
    }
};


#endif //BLOCKGAME_MESHER_H
