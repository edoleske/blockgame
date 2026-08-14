#ifndef BLOCKGAME_ITEMBLOCK_H
#define BLOCKGAME_ITEMBLOCK_H

#include "item.h"
#include "game/block/block.h"


class ItemBlock : public Item {
public:
    explicit ItemBlock(BlockID id);

    std::unique_ptr<Item> clone() const override;

    int getTexture() const override;

    void onUse() override;

    const Block* getBlock() const;

private:
    BlockID block;
};


#endif //BLOCKGAME_ITEMBLOCK_H
