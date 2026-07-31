#ifndef BLOCKGAME_BLOCK_H
#define BLOCKGAME_BLOCK_H


#include "common.h"
#include "utils/vertex.h"
#include "blockType.h"


class Block {
public:
    Block() = default;

    explicit Block(BlockID id);

    explicit Block(BlockID id, uint8_t state);

    BlockID getID() const;

    BlockType getType() const;

    uint8_t getState() const;

    static unordered_map<BlockFace, vector<Vertex>> blockFaceVertices;

    static vector<Vertex> billboardVertices;

private:
    uint16_t id{0};
    uint8_t state{0};
};


#endif //BLOCKGAME_BLOCK_H
