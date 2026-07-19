#ifndef BLOCKGAME_CROSSHAIR_H
#define BLOCKGAME_CROSSHAIR_H
#include "../uiElement.h"


class Crosshair : public UIElement {
public:
    Crosshair();

    explicit Crosshair(string id, const vec2& position, const vec2& size, bool hidden);

    void updateWindowSize(int width, int height) override;
};


#endif //BLOCKGAME_CROSSHAIR_H
