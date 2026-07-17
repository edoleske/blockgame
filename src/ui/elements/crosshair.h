#ifndef BLOCKGAME_CROSSHAIR_H
#define BLOCKGAME_CROSSHAIR_H
#include "../uiElement.h"


class Crosshair : public UIElement {
public:
    Crosshair() = default;

    Crosshair(const vec2& position, const vec2& size) : UIElement(position, size) {}

    void updateWindowSize(int width, int height) override;
};


#endif //BLOCKGAME_CROSSHAIR_H
