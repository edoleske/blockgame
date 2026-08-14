#include "itemBlock.h"

#include "game/block/blockDictionary.h"

ItemBlock::ItemBlock(const BlockID id): Item(BlockDictionary::getInstance()->get(id).name), block(id) {}

std::unique_ptr<Item> ItemBlock::clone() const {
    return make_unique<ItemBlock>(*this);
}

int ItemBlock::getTexture() const {
    return getBlock().faceTextures[0];
}

void ItemBlock::onUse() {
    // Try to place block
}

Block ItemBlock::getBlock() const {
    return BlockDictionary::getInstance()->get(block);
}
