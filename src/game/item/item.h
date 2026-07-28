#ifndef BLOCKGAME_ITEM_H
#define BLOCKGAME_ITEM_H

#include "common.h"


class Item {
public:
    Item() = default;

    virtual void onUse();

    uint8_t STACK_SIZE = 64;
};


#endif //BLOCKGAME_ITEM_H
