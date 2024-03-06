#ifndef BLOCKGAME_BLOCK_H
#define BLOCKGAME_BLOCK_H


#include "../common.h"
#include "../utils/vertex.h"

enum BlockFace {
    TOP, BOTTOM, LEFT, RIGHT, FRONT, BACK
};

enum BlockType {
    AIR, STONE
};

class Block {
public:
    Block() = default;
    explicit Block(BlockType type);

    BlockType getType() const;

    static void addBlockFaceVertices(vector<Vertex> &vertices, BlockFace face, u8vec3 &chunkPosition);
private:
    BlockType type = BlockType::AIR;
};


#endif //BLOCKGAME_BLOCK_H
