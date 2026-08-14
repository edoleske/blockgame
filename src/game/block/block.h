#ifndef BLOCKGAME_BLOCK_H
#define BLOCKGAME_BLOCK_H


#include "common.h"
#include "utils/vertex.h"

// Defines common indices for block faces
// TOP:   +y, BOTTOM: -y
// RIGHT: +x,   LEFT: -x
// FRONT: +z,   BACK: -z
enum class BlockFace : uint8_t {
    TOP, BOTTOM, RIGHT, LEFT, FRONT, BACK
};


class Block {
public:
    Block() = default;

    explicit Block(BlockID id);

    uint16_t getLayer(BlockFace face) const;

    bool operator==(const Block& other) const;

    bool isCollidable() const;

    uint16_t id = 0;
    string name;
    bool opaque = false;
    bool isBillboard = false;

    // Texture Array Layer indexed by BlockFace
    // Billboard textures expect these to be identical and use first index
    std::array<uint16_t, 6> faceTextures = {0, 0, 0, 0, 0, 0};

    static unordered_map<BlockFace, vector<Vertex>> blockFaceVertices;

    static vector<Vertex> billboardVertices;

    static constexpr uint16_t fnv1a(const string& s) {
        if (s.empty()) return 0;
        int hash = -2128831035;
        for (auto c : s) {
            hash ^= static_cast<int>(c);
            hash *= 16777619;
        }
        return (hash >> 16) ^ (hash & 0xFFFF);
    }
};

template <>
struct std::hash<Block> {
    uint16_t operator()(const Block& type) const noexcept {
        return Block::fnv1a(type.name);
    }
};


#endif //BLOCKGAME_BLOCK_H
