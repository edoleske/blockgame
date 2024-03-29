#ifndef BLOCKGAME_BLOCK_H
#define BLOCKGAME_BLOCK_H


#include "common.h"
#include "utils/vertex.h"
#include "blockTexture.h"

enum class BlockFace : uint8_t {
    TOP, BOTTOM, LEFT, RIGHT, FRONT, BACK
};

enum class BlockType : uint16_t {
    AIR, DIRT, GRASS, BEDROCK, STONE, WATER, LEAVES, LOG, FLOWER
};

class Block {
public:
    Block() = default;

    explicit Block(BlockType type);
    explicit Block(BlockType type, uint8_t state);

    BlockType getType() const;

    uint8_t getState() const;

    bool isOpaque() const;

    bool isDifferentTransparent(const BlockType& otherType) const;

    static bool isBlockTypeOpaque(BlockType type);

    static bool isBlockTypeBillboard(BlockType type);

    static unordered_map<BlockFace, vector<Vertex>> blockFaceVertices;

    static vector<Vertex> billboardVertices;

    static BlockTextureName getBlockFaceTexture(BlockType type, BlockFace face);

private:
    uint16_t data{ 0 };
};


#endif //BLOCKGAME_BLOCK_H
