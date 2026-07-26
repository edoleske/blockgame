#include "crosshair.h"

Crosshair::Crosshair() : UIElement("crosshair", UIT_CROSSHAIR, vec2(0.0f), vec2(16.0f), vec2(0.0f), false) {}

Crosshair::Crosshair(string id, const vec2& position, const vec2& size, const bool hidden)
    : UIElement(std::move(id), UIT_CROSSHAIR, position, size, vec2(0.0f), hidden) {}

void Crosshair::updateWindowSize(const int width, const int height) {
    // Center crosshair
    const auto half_size = size / 2.0f;
    position = vec2(static_cast<float>(width) / 2.0f, static_cast<float>(height) / 2.0f) - half_size;
}
