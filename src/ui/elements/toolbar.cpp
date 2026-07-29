#include "toolbar.h"

Toolbar::Toolbar(UIElement* highlight, const std::array<UIElement*, Inventory::MAX_HOTBAR_SLOTS>& itemSprites)
    : UIElement(UIElementConfig{
          .id = "toolbar", .textureName = UIT_TOOLBAR, .position = vec2(0.0f),
          .size = vec2(200.0f, 20.0f), .scale = 3.0f, .origin = vec2(0.5f, 1.0f), .hidden = false,
          .centerX = true
      }), itemSprites(itemSprites), toolbarHighlight(highlight) {}

void Toolbar::updateFromInventory(const Inventory& inventory) {
    if (const auto selected = inventory.getSelected();
        selected != highlightPosition) {
        setHighlightPosition(selected);
    }

    for (int i = 0; i < itemSprites.size(); i++) {
        const auto stack = inventory.getItemStack(i);
        if (stack == nullptr) continue;

        itemSprites[i]->textureName = stack->item->getName() != "0" ? UIT_PLACEHOLDER : UIT_NONE;
    }
}

void Toolbar::updateWindowSize(const int width, const int height) {
    UIElement::updateWindowSize(width, height);

    // Set toolbar to bottom of new height
    position.y = height;

    // Adjust highlight position
    toolbarHighlight->position = vec2((width / 2) - (size.x * scale / 2), height);

    // Recalculate all toolbar item sprite positions
    for (int i = 0; i < itemSprites.size(); i++) {
        const auto item = itemSprites[i];
        item->position = vec2((width / 2) - (size.x * scale / 2) + (i * item->size.x * item->scale), height);
    }
}

int Toolbar::getHighlightPosition() const {
    return highlightPosition;
}

void Toolbar::setHighlightPosition(const int index) {
    toolbarHighlight->position.x -= highlightPosition * toolbarHighlight->size.x * scale;
    highlightPosition = index;
    toolbarHighlight->position.x += highlightPosition * toolbarHighlight->size.x * scale;
}
