#include "item.h"

Item::Item(string name) : name(std::move(name)) {}

string Item::getName() const {
    return name;
}

std::unique_ptr<Item> Item::clone() const {
    return std::make_unique<Item>(name);
}

void Item::onUse() {}
