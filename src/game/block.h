#ifndef BLOCKGAME_BLOCK_H
#define BLOCKGAME_BLOCK_H


#include "common.h"
#include "utils/vertex.h"
#include "blockTexture.h"

enum class BlockFace : uint8_t {
    TOP, BOTTOM, LEFT, RIGHT, FRONT, BACK
};

enum class BlockType : uint16_t {
    AIR, DIRT, GRASS, BEDROCK, STONE
};

class Block {
public:
    Block() = default;

    explicit Block(BlockType type);
    explicit Block(BlockType type, uint8_t state);

    BlockType getType() const;

    uint8_t getState() const;

    static unordered_map<BlockFace, vector<Vertex>> blockFaceVertices;

    static BlockTextureName getBlockFaceTexture(BlockType type, BlockFace face);

private:
    BlockType type = BlockType::AIR;
    uint8_t state{ 0 };
};


#endif //BLOCKGAME_BLOCK_H
