#include "blockType.h"

uint16_t BlockType::getLayer(BlockFace face) const {
    return faceTextures[static_cast<int>(face)];
}
