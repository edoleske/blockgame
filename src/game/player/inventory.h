#ifndef BLOCKGAME_INVENTORY_H
#define BLOCKGAME_INVENTORY_H
#include "game/item/itemStack.h"


class Inventory {
public:
    Inventory();

    static constexpr int MAX_HOTBAR_SLOTS = 10;

    int getSelected() const;

    void setSelected(int index);

    void insert(ItemStack& itemIn);

    const ItemStack* getItemStack(int index) const;

    void setItemStack(int index, const ItemStack& itemIn);

private:
    int selected = 0;

    std::array<ItemStack, MAX_HOTBAR_SLOTS> hotbar{};
};


#endif //BLOCKGAME_INVENTORY_H
