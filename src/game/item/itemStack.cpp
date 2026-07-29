#include "itemStack.h"

#include <utility>

ItemStack::ItemStack() : item(make_unique<Item>("0")), amount(0) {}

ItemStack::ItemStack(const ItemStack& other) : item(other.item->clone()), amount(other.amount) {}

ItemStack::ItemStack(unique_ptr<Item> item, const int amount) : item(std::move(item)), amount(amount) {}

ItemStack& ItemStack::operator=(const ItemStack& other) {
    if (this != &other) {
        this->item = other.item->clone();
        this->amount = other.amount;
    }

    return *this;
}
