#ifndef BLOCKGAME_ITEMSTACK_H
#define BLOCKGAME_ITEMSTACK_H
#include "item.h"


class ItemStack {
public:
    ItemStack(Item item, int amount);
    
private:
    Item item;
    int amount;
};


#endif //BLOCKGAME_ITEMSTACK_H
