#include "uiElement.h"

#include <utility>
#include "uiBatch.h"

UIElement::UIElement(string id) : position(0.0f), size(1.0f), id(std::move(id)) {}

UIElement::UIElement(string id, const UITextureName textureName, const vec2& position, const vec2& size,
                     const vec2& origin, const bool hidden) :
    textureName(textureName), position(position), size(size), origin(origin), hidden(hidden), id(std::move(id)) {}

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

void UIElement::generateVertices(const unique_ptr<UIBatch>& batch,
                                 const unique_ptr<UITextureAtlas>& textureAtlas) const {
    const auto topLeft = position - (origin * size);
    const auto uv = textureAtlas->getUV(textureName);
    batch->insertQuad(topLeft, size, vec2(uv.x, uv.y), vec2(uv.z, uv.w));
}

void UIElement::updateWindowSize(int, int) {}
