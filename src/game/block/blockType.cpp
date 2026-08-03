#include "blockType.h"



uint16_t BlockType::getLayer(BlockFace face) const {
    return faceTextures[static_cast<int>(face)];
}

bool BlockType::operator==(const BlockType& other) const {
    return id == other.id;
}

bool BlockType::isCollidable() const {
    return opaque || isBillboard;
}
