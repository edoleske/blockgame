#ifndef BLOCKGAME_BLOCK_H
#define BLOCKGAME_BLOCK_H


#include "common.h"
#include "utils/vertex.h"
#include "blockType.h"


class Block {
public:
    Block() = default;

    explicit Block(BlockID id);

    BlockID getID() const;

    BlockType getType() const;

    static unordered_map<BlockFace, vector<Vertex>> blockFaceVertices;

    static vector<Vertex> billboardVertices;

private:
    uint16_t id{0};
};


#endif //BLOCKGAME_BLOCK_H
