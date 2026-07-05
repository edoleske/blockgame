#ifndef BLOCKGAME_UIELEMENT_H
#define BLOCKGAME_UIELEMENT_H

#include "common.h"
#include "gl/texture.h"


class UIElement {
public:
    UIElement(const shared_ptr<Texture> &texture);

    UIElement(const shared_ptr<Texture> &texture, const vec2 &position, const vec2 &size);

    mat4 getModel() const;

    void setPosition(float x, float y);

    void setSize(float x, float y);

    shared_ptr<Texture> getTexture() const;

    virtual void updateWindowSize(int width, int height);
private:
    vec2 position;
    vec2 size;
    mat4 model = mat4(1);

    void recalculateModel();

    shared_ptr<Texture> _texture;
};


#endif //BLOCKGAME_UIELEMENT_H
