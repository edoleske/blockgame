#include "itemBlock.h"

ItemBlock::ItemBlock(const Block block) : Item(std::to_string(static_cast<int>(block.getType()))), block(block) {}

std::unique_ptr<Item> ItemBlock::clone() const {
    return make_unique<ItemBlock>(*this);
}

void ItemBlock::onUse() {
    // Try to place block
}
