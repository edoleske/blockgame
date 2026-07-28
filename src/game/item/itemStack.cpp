#include "itemStack.h"

#include <utility>

ItemStack::ItemStack(Item item, const int amount) : item(std::move(item)), amount(amount) {}
