#include "uiElement.h"

#include <utility>
#include "uiBatch.h"

UIElement::UIElement(string id) : position(0.0f), size(1.0f), id(std::move(id)) {}

UIElement::UIElement(string id, const UITextureName textureName, const vec2& position, const vec2& size,
                     const bool hidden) : textureName(textureName), position(position), size(size), hidden(hidden),
                                          id(std::move(id)) {}

string UIElement::getID() const {
    return id;
}

void UIElement::setPosition(const float x, const float y) {
    position = vec2(x, y);
}

void UIElement::setSize(const float x, const float y) {
    size = vec2(x, y);
}

void UIElement::generateVertices(const unique_ptr<UIBatch>& batch) const {
    const auto [x, y, w, h] = uiTextureInfoMap[textureName];
    batch->insertQuad(position, size, vec2(x / UI_TEXTURE_WIDTH, y / UI_TEXTURE_HEIGHT), vec2((x + w) / UI_TEXTURE_WIDTH, (y + h) / UI_TEXTURE_HEIGHT));
}

void UIElement::updateWindowSize(int, int) {}
