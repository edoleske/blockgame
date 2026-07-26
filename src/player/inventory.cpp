#include "inventory.h"

Inventory::Inventory() = default;

int Inventory::getSelected() const {
    return selected;
}

void Inventory::setSelected(const int index) {
    selected = (index % MAX_HOTBAR_SLOTS + MAX_HOTBAR_SLOTS) % MAX_HOTBAR_SLOTS;
}
