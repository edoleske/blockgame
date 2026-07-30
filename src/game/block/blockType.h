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

    uint16_t id = 0;
    bool opaque = true;
    bool isBillboard = false;
    // Texture Array Layer indexed by BlockFace
    std::array<uint16_t, 6> faceTextures = {0, 0, 0, 0, 0, 0};
};


#endif //BLOCKGAME_BLOCKTYPE_H
