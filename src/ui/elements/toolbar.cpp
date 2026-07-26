#include "toolbar.h"

Toolbar::Toolbar(UIElement* highlight) : UIElement(UIElementConfig{
                                             .id = "toolbar", .textureName = UIT_TOOLBAR, .position = vec2(0.0f),
                                             .size = vec2(200.0f, 20.0f), .scale = 3.0f, .origin = vec2(0.5f, 1.0f), .hidden = false,
                                             .centerX = true
                                         }), toolbarHighlight(highlight) {}

void Toolbar::updateWindowSize(const int width, const int height) {
    UIElement::updateWindowSize(width, height);
    position.y = height;
    toolbarHighlight->position = vec2((width / 2) - (size.x * scale / 2), height);
}

int Toolbar::getHighlightPosition() const {
    return highlightPosition;
}

void Toolbar::setHighlightPosition(const int index) {
    toolbarHighlight->position.x -= highlightPosition * toolbarHighlight->size.x * scale;
    highlightPosition = index;
    toolbarHighlight->position.x += highlightPosition * toolbarHighlight->size.x * scale;
}
