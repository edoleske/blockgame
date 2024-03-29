#include "block.h"

Block::Block(BlockType type) {
    data = static_cast<uint16_t>(type) << 4;
}

Block::Block(BlockType type, uint8_t state) {
    data = static_cast<uint16_t>(type) << 4;
    data |= (state & 0xF);
}

BlockType Block::getType() const {
    return static_cast<BlockType>(data >> 4);
}

uint8_t Block::getState() const {
    return data & 0xF;
}

bool Block::isOpaque() const {
    auto type = getType();
    return Block::isBlockTypeOpaque(type);
}

bool Block::isDifferentTransparent(const BlockType& otherType) const {
    auto type = getType();
    return !Block::isBlockTypeOpaque(type) && type != otherType;
}

bool Block::isBlockTypeOpaque(BlockType type) {
    return type != BlockType::AIR && type != BlockType::WATER;
}

unordered_map<BlockFace, vector<Vertex>> Block::blockFaceVertices = {
        {BlockFace::TOP,    {
                                    {{0, 1, 0}, {0, 0}},
                                    {{1, 1, 0}, {1, 0}},
                                    {{1, 1, 1}, {1, 1}},
                                    {{0, 1, 1}, {0, 1}},
                            }},
        {BlockFace::BOTTOM, {
                                    {{0, 0, 0}, {0, 0}},
                                    {{0, 0, 1}, {0, 1}},
                                    {{1, 0, 1}, {1, 1}},
                                    {{1, 0, 0}, {1, 0}},
                            }},
        {BlockFace::LEFT,   {
                                    {{0, 1, 1}, {0, 0}},
                                    {{0, 0, 1}, {0, 1}},
                                    {{0, 0, 0}, {1, 1}},
                                    {{0, 1, 0}, {1, 0}},
                            }},
        {BlockFace::RIGHT,  {
                                    {{1, 1, 1}, {1, 0}},
                                    {{1, 1, 0}, {0, 0}},
                                    {{1, 0, 0}, {0, 1}},
                                    {{1, 0, 1}, {1, 1}},
                            }},
        {BlockFace::FRONT,  {
                                    {{0, 0, 1}, {0, 1}},
                                    {{0, 1, 1}, {0, 0}},
                                    {{1, 1, 1}, {1, 0}},
                                    {{1, 0, 1}, {1, 1}},
                            }},
        {BlockFace::BACK,   {
                                    {{0, 0, 0}, {1, 1}},
                                    {{1, 0, 0}, {0, 1}},
                                    {{1, 1, 0}, {0, 0}},
                                    {{0, 1, 0}, {1, 0}},
                            }}
};

BlockTextureName Block::getBlockFaceTexture(BlockType type, BlockFace face) {
    switch (type) {
        case BlockType::DIRT:
            return BlockTextureName::DIRT;
        case BlockType::GRASS:
            switch (face) {
                case BlockFace::TOP:
                    return BlockTextureName::GRASS;
                case BlockFace::BOTTOM:
                    return BlockTextureName::DIRT;
                default:
                    return BlockTextureName::GRASS_SIDE;
            }
        case BlockType::BEDROCK:
            return BlockTextureName::BEDROCK;
        case BlockType::STONE:
            return BlockTextureName::STONE;
        case BlockType::WATER:
            return BlockTextureName::WATER;
        default:
            return BlockTextureName::DIRT;
    }
}
