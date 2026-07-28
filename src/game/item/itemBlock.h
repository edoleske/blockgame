#ifndef BLOCKGAME_ITEMBLOCK_H
#define BLOCKGAME_ITEMBLOCK_H
#include "item.h"
#include "game/block/block.h"


class ItemBlock : public Item {
public:
    ItemBlock();

    void onUse() override;

private:
    Block block;
};


#endif //BLOCKGAME_ITEMBLOCK_H
