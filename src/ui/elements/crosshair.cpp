#include "crosshair.h"

void Crosshair::updateWindowSize(int width, int height) {
    // Center crosshair
    auto half_size = size / 2.0f;
    position = vec2(width / 2.0f, height / 2.0f) - half_size;
}
