#include "itemSprite.h"

#include <format>

#include "log.h"
#include "ui/uiRenderer.h"

ItemSprite::ItemSprite(UIElementConfig config) : UIElement(config),
                                                 amountLabel(make_unique<TextBox>(
                                                     std::format("{}Amount", config.id), config.font)) {}

void ItemSprite::onRender(const UIRenderer& renderer, const UIRenderPass pass) const {
    if (pass == UI_MAIN || item == nullptr || item->getTexture() == 0) return;

    if (pass == UI_TEXT) {
        if (amount < 2) return;
        amountLabel->text = std::format("{}", amount);
        amountLabel->onRender(renderer, pass);
        return;
    }

    const auto topLeft = position - origin * (size * scale);
    const auto margin = vec2(2.0f) * scale;
    const auto layer = static_cast<float>(item->getTexture());
    renderer.getBatch()->insertQuad({
        .position = topLeft + margin, .size = size * scale - margin * 2.0f, .layer = layer,
    });
}

void ItemSprite::updatePosition(const vec2 newPos) {
    position = newPos;
    const auto margin = 2.0f * scale;
    amountLabel->position = vec2(newPos.x + margin, newPos.y + margin - size.y * scale);
}

void ItemSprite::setItem(Item* newItem) {
    item = newItem;
}
