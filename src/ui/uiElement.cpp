#include "uiElement.h"

UIElement::UIElement(const shared_ptr<Texture> &texture) : position(0.0f), size(1.0f), _texture(texture) {
    recalculateModel();
}

UIElement::UIElement(const shared_ptr<Texture>& texture, const vec2& position, const vec2& size): position(position), size(size), _texture(texture) {
    recalculateModel();
}

mat4 UIElement::getModel() const {
    return model;
}

void UIElement::setPosition(float x, float y) {
    position = vec2(x, y);
    recalculateModel();
}

void UIElement::setSize(float x, float y) {
    size = vec2(x, y);
    recalculateModel();
}

shared_ptr<Texture> UIElement::getTexture() const {
    return _texture;
}

void UIElement::updateWindowSize(int width, int height) {
    // Temporary centering
    auto half_size = size / 2.0f;
    position = vec2(width / 2.0f, height / 2.0f) - half_size;

    recalculateModel();
}

void UIElement::recalculateModel() {
    model = mat4(1);
    model = glm::translate(model, vec3(position, 0.0f));
    model = glm::scale(model, vec3(size, 1.0f));
}
