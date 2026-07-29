#include "uiElement.h"

#include <utility>
#include "uiBatch.h"

UIElement::UIElement(string id) : UIElement(UIElementConfig(id = std::move(id))) {}

UIElement::UIElement(UIElementConfig config) : textureName(config.textureName), position(config.position),
                                               size(config.size), scale(config.scale), origin(config.origin),
                                               centerX(config.centerX), centerY(config.centerY), hidden(config.hidden),
                                               renderPass(config.renderPass), id(std::move(config.id)) {}

string UIElement::getID() const {
    return id;
}

void UIElement::setPosition(const float x, const float y) {
    position = vec2(x, y);
}

void UIElement::setSize(const float x, const float y) {
    size = vec2(x, y);
}

void UIElement::setOrigin(const float x, const float y) {
    origin = vec2(x, y);
}

void UIElement::generateVertices(
    const unique_ptr<UIBatch>& batch,
    const unique_ptr<UITextureAtlas>& textureAtlas) const {
    const auto topLeft = position - origin * (size * scale);
    const auto uv = textureAtlas->getUV(textureName);
    batch->insertQuad(topLeft, size * scale, vec2(uv.x, uv.y), vec2(uv.z, uv.w));
}

void UIElement::updateWindowSize(const int width, const int height) {
    if (centerX) {
        position.x = static_cast<float>(width) / 2;
    }
    if (centerY) {
        position.y = static_cast<float>(height) / 2;
    }
}
