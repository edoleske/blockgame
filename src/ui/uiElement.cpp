#include "uiElement.h"
#include "uiBatch.h"

UIElement::UIElement() : position(0.0f), size(1.0f) {}

UIElement::UIElement(const vec2& position, const vec2& size) : position(position), size(size) {}

void UIElement::setPosition(float x, float y) {
    position = vec2(x, y);
}

void UIElement::setSize(float x, float y) {
    size = vec2(x, y);
}

void UIElement::generateVertices(const unique_ptr<UIBatch>& batch) const {
    batch->insertQuad(position, size);
}

void UIElement::updateWindowSize(int width, int height) {}
