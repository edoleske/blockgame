#include "block.h"

Block::Block(BlockType type) : type(type) {}

BlockType Block::getType() const {
    return type;
}

map<BlockFace, vector<Vertex>> Block::blockFaceVertices = {
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
        default:
            return BlockTextureName::DIRT;
    }
}
