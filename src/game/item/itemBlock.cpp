#include "itemBlock.h"

#include "game/block/blockDictionary.h"

ItemBlock::ItemBlock(const BlockID id): Item(BlockDictionary::getInstance()->get(id).name), block(id) {}

std::unique_ptr<Item> ItemBlock::clone() const {
    return make_unique<ItemBlock>(*this);
}

void ItemBlock::onUse() {
    // Try to place block
}

BlockType ItemBlock::getBlockType() const {
    return BlockDictionary::getInstance()->get(block);
}
