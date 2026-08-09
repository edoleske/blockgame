#include "toolbar.h"

Toolbar::Toolbar(const shared_ptr<Font>& font)
    : UIElement(UIElementConfig{
          .id = "toolbar", .textureName = UIT_TOOLBAR, .position = vec2(0.0f),
          .size = vec2(200.0f, 20.0f), .scale = 3.0f, .origin = vec2(0.5f, 1.0f), .hidden = false,
          .centerX = true
      }) {
    std::array<UIElement*, Inventory::MAX_HOTBAR_SLOTS> toolbarItemSprites{};
    for (int i = 0; i < itemSprites.size(); i++) {
        itemSprites[i] = make_unique<ItemSprite>(UIElementConfig{
            .id = "toolbarItemSprite" + std::to_string(i),
            .textureName = UIT_NONE,
            .size = vec2(20.0f),
            .scale = 3.0f,
            .origin = vec2(0.0f, 1.0f),
            .font = font
        });
    }

    highlight = make_unique<UIElement>(UIElementConfig{
        .id = "toolbarHighlight", .textureName = UIT_HIGHLIGHT, .position = vec2(0.0f), .size = vec2(20.0f),
        .scale = 3.0f, .origin = vec2(0.0f, 1.0f)
    });
}

void Toolbar::onRender(const UIRenderer& renderer, const UIRenderPass pass) const {
    UIElement::onRender(renderer, pass);

    for (const auto& sprite : itemSprites) {
        sprite->onRender(renderer, pass);
    }
    highlight->onRender(renderer, pass);
}

void Toolbar::updateFromInventory(const Inventory& inventory) {
    if (const auto selected = inventory.getSelected();
        selected != highlightPosition) {
        setHighlightPosition(selected);
    }

    for (int i = 0; i < itemSprites.size(); i++) {
        const auto stack = inventory.getItemStack(i);
        const auto item = stack == nullptr ? nullptr : stack->item.get();
        const auto amount = stack == nullptr ? 0 : stack->amount;

        itemSprites[i]->setItem(item);
        itemSprites[i]->amount = amount;
    }
}

void Toolbar::updateWindowSize(const int width, const int height) {
    UIElement::updateWindowSize(width, height);

    // Set toolbar to bottom of new height
    position.y = height;

    // Adjust highlight position
    highlight->position = vec2((width / 2) - (size.x * scale / 2), height);

    // Recalculate all toolbar item sprite positions
    for (int i = 0; i < itemSprites.size(); i++) {
        auto x = (width / 2) - (size.x * scale / 2) + (i * itemSprites[i]->size.x * itemSprites[i]->scale);
        itemSprites[i]->updatePosition(vec2(x, height));
    }
}

int Toolbar::getHighlightPosition() const {
    return highlightPosition;
}

void Toolbar::setHighlightPosition(const int index) {
    highlight->position.x -= highlightPosition * highlight->size.x * scale;
    highlightPosition = index;
    highlight->position.x += highlightPosition * highlight->size.x * scale;
}
