#ifndef BLOCKGAME_BLOCKTYPE_H
#define BLOCKGAME_BLOCKTYPE_H

#include "common.h"

// Defines common indices for block faces
// TOP:   +y, BOTTOM: -y
// RIGHT: +x,   LEFT: -x
// FRONT: +z,   BACK: -z
enum class BlockFace : uint8_t {
    TOP, BOTTOM, RIGHT, LEFT, FRONT, BACK
};


class BlockType {
public:
    uint16_t getLayer(BlockFace face) const;

    bool operator==(const BlockType& other) const;

    bool isCollidable() const;

    uint16_t id = 0;
    string name;
    bool opaque = false;
    bool isBillboard = false;
    // Texture Array Layer indexed by BlockFace
    // Billboard textures expect these to be identical and use first index
    std::array<uint16_t, 6> faceTextures = {0, 0, 0, 0, 0, 0};

    static constexpr BlockID fnv1a(const string& s) {
        BlockID hash = -2128831035;
        for (auto c : s) {
            hash ^= static_cast<BlockID>(c);
            hash *= 16777619;
        }
        return (hash >> 16) ^ (hash & 0xFFFF);
    }
};

template <>
struct std::hash<BlockType> {
    BlockID operator()(const BlockType& type) const noexcept {
        return BlockType::fnv1a(type.name);
    }
};

#endif //BLOCKGAME_BLOCKTYPE_H
