#include "block.h"

#include "blockDictionary.h"

Block::Block(const BlockID id) : id(id) {}

Block::Block(const BlockID id, const uint8_t state) : id(id), state(state) {}

BlockID Block::getID() const {
    return id;
}

BlockType Block::getType() const {
    return BlockDictionary::getInstance()->get(getID());
}

uint8_t Block::getState() const {
    return state;
}

unordered_map<BlockFace, vector<Vertex>> Block::blockFaceVertices = {
    {
        BlockFace::TOP, {
            {.position = {0, 1, 0}, .uv = {0, 0}},
            {.position = {1, 1, 0}, .uv = {1, 0}},
            {.position = {1, 1, 1}, .uv = {1, 1}},
            {.position = {0, 1, 1}, .uv = {0, 1}},
        }
    },
    {
        BlockFace::BOTTOM, {
            {.position = {0, 0, 0}, .uv = {0, 0}},
            {.position = {0, 0, 1}, .uv = {0, 1}},
            {.position = {1, 0, 1}, .uv = {1, 1}},
            {.position = {1, 0, 0}, .uv = {1, 0}},
        }
    },
    {
        BlockFace::LEFT, {
            {.position = {0, 1, 1}, .uv = {0, 0}},
            {.position = {0, 0, 1}, .uv = {0, 1}},
            {.position = {0, 0, 0}, .uv = {1, 1}},
            {.position = {0, 1, 0}, .uv = {1, 0}},
        }
    },
    {
        BlockFace::RIGHT, {
            {.position = {1, 0, 1}, .uv = {1, 1}},
            {.position = {1, 1, 1}, .uv = {1, 0}},
            {.position = {1, 1, 0}, .uv = {0, 0}},
            {.position = {1, 0, 0}, .uv = {0, 1}},
        }
    },
    {
        BlockFace::FRONT, {
            {.position = {0, 0, 1}, .uv = {0, 1}},
            {.position = {0, 1, 1}, .uv = {0, 0}},
            {.position = {1, 1, 1}, .uv = {1, 0}},
            {.position = {1, 0, 1}, .uv = {1, 1}},
        }
    },
    {
        BlockFace::BACK, {
            {.position = {0, 0, 0}, .uv = {1, 1}},
            {.position = {1, 0, 0}, .uv = {0, 1}},
            {.position = {1, 1, 0}, .uv = {0, 0}},
            {.position = {0, 1, 0}, .uv = {1, 0}},
        }
    }
};

vector<Vertex> Block::billboardVertices = {
    {.position = {0, 0, 0}, .uv = {0, 1}},
    {.position = {0, 1, 0}, .uv = {0, 0}},
    {.position = {1, 1, 1}, .uv = {1, 0}},
    {.position = {1, 0, 1}, .uv = {1, 1}},
    {.position = {0, 0, 0}, .uv = {1, 1}},
    {.position = {1, 0, 1}, .uv = {0, 1}},
    {.position = {1, 1, 1}, .uv = {0, 0}},
    {.position = {0, 1, 0}, .uv = {1, 0}},
    {.position = {0, 1, 1}, .uv = {0, 0}},
    {.position = {0, 0, 1}, .uv = {0, 1}},
    {.position = {1, 0, 0}, .uv = {1, 1}},
    {.position = {1, 1, 0}, .uv = {1, 0}},
    {.position = {0, 1, 1}, .uv = {1, 0}},
    {.position = {1, 1, 0}, .uv = {0, 0}},
    {.position = {1, 0, 0}, .uv = {0, 1}},
    {.position = {0, 0, 1}, .uv = {1, 1}},
};
