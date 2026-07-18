#ifndef BLOCKGAME_UIELEMENT_H
#define BLOCKGAME_UIELEMENT_H

#include "common.h"


class UIBatch;

struct UIVertex {
    vec2 position;
    vec2 uv;
};


class UIElement {
public:
    UIElement();

    UIElement(const vec2& position, const vec2& size);

    virtual ~UIElement() = default;

    void setPosition(float x, float y);

    void setSize(float x, float y);

    // Adds all quad vertices to UI batch buffer
    virtual void generateVertices(const unique_ptr<UIBatch>& batch) const;

    virtual void updateWindowSize(int width, int height);

    vec2 position;
    vec2 size;

    bool hidden = false;
};


#endif //BLOCKGAME_UIELEMENT_H
