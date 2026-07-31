#include "blockType.h"

uint16_t BlockType::getLayer(BlockFace face) const {
    return faceTextures[static_cast<int>(face)];
}

bool BlockType::operator==(const int i) const {
    return static_cast<int>(id) == i;
}

bool BlockType::isCollidable() const {
    return opaque || isBillboard;
}
