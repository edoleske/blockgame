#include "crosshair.h"

#include <utility>

Crosshair::Crosshair() : UIElement(UIElementConfig{
    .id = "crosshair",
    .textureName = UIT_CROSSHAIR,
    .size = vec2(16.0f),
    .origin = vec2(0.5f, 0.5f),
    .centerX = true,
    .centerY = true
}) {}

Crosshair::Crosshair(UIElementConfig config) : UIElement(std::move(config)) {}
