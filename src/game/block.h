#ifndef BLOCKGAME_BLOCK_H
#define BLOCKGAME_BLOCK_H


#include "common.h"
#include "utils/vertex.h"
#include "blockTexture.h"

enum class BlockFace {
    TOP, BOTTOM, LEFT, RIGHT, FRONT, BACK
};

enum class BlockType {
    AIR, DIRT, STONE
};

class Block {
public:
    Block() = default;

    explicit Block(BlockType type);

    BlockType getType() const;

    static void addBlockFaceVertices(vector<Vertex>& vertices, BlockFace face, const u8vec3& chunkPosition);

    static BlockTextureName getBlockFaceTexture(BlockType type, BlockFace face);

private:
    BlockType type = BlockType::AIR;
};


#endif //BLOCKGAME_BLOCK_H
