#include "inventory.h"

Inventory::Inventory() = default;

int Inventory::getSelected() const {
    return selected;
}

void Inventory::setSelected(const int index) {
    selected = (index % MAX_HOTBAR_SLOTS + MAX_HOTBAR_SLOTS) % MAX_HOTBAR_SLOTS;
}

const ItemStack* Inventory::getItemStack(const int index) const {
    if (index < 0 || index >= MAX_HOTBAR_SLOTS) { return nullptr; }

    return &hotbar[index];
}

void Inventory::setItemStack(const int index, const ItemStack& itemIn) {
    if (index < 0 || index >= MAX_HOTBAR_SLOTS) return;

    hotbar[index] = itemIn;
}

void Inventory::insert(ItemStack& itemIn) {
    if (itemIn.amount <= 0) return;

    for (auto& stack : hotbar) {
        if (stack.amount == stack.item->STACK_SIZE) continue;

        if (stack.item->getName() == itemIn.item->getName() || stack.amount == 0) {
            stack.item = std::move(itemIn.item);
            const auto remainder = stack.item->STACK_SIZE - stack.amount;

            if (remainder < itemIn.amount) {
                itemIn.amount -= remainder;
                stack.amount += remainder;
            } else {
                stack.amount += itemIn.amount;
                itemIn.amount = 0;
                break;
            }
        }
    }
}
