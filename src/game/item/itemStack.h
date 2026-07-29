#ifndef BLOCKGAME_ITEMSTACK_H
#define BLOCKGAME_ITEMSTACK_H

#include "common.h"
#include "item.h"


class ItemStack {
public:
    ItemStack();

    ItemStack(const ItemStack& other);

    ItemStack(unique_ptr<Item> item, int amount);

    ItemStack& operator=(const ItemStack& other);

    unique_ptr<Item> item;
    int amount;
};


#endif //BLOCKGAME_ITEMSTACK_H
