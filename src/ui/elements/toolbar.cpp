#include "toolbar.h"

Toolbar::Toolbar(UIElement* highlight) : UIElement("toolbar", UIT_TOOLBAR, vec2(0.0f), vec2(600.0f, 60.0f),
                                                   vec2(0.5f, 1.0f), false), toolbarHighlight(highlight) {}

void Toolbar::updateWindowSize(const int width, const int height) {
    position = vec2(width / 2, height);
    toolbarHighlight->position = vec2((width / 2) - (size.x / 2), height);
}

int Toolbar::getSelected() const {
    return selected;
}

void Toolbar::setSelected(const int index) {
    toolbarHighlight->position.x -= selected * toolbarHighlight->size.x;
    selected = index;
    toolbarHighlight->position.x += selected * toolbarHighlight->size.x;
}
