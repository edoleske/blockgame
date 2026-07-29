#ifndef BLOCKGAME_ITEM_H
#define BLOCKGAME_ITEM_H

#include "common.h"


class Item {
public:
    explicit Item(string name);

    virtual ~Item() = default;

    virtual std::unique_ptr<Item> clone() const;

    virtual void onUse();

    string getName() const;

    uint8_t STACK_SIZE = 64;

private:
    string name;
};


#endif //BLOCKGAME_ITEM_H
