#ifndef BLOCKGAME_UIELEMENT_H
#define BLOCKGAME_UIELEMENT_H

#include "common.h"
#include "uiTextureAtlas.h"

class UIBatch;

enum UI_RENDERPASS {
    UI_MAIN, UI_TEXT
};


class UIElement {
public:
    explicit UIElement(string id);

    explicit UIElement(string id, UITextureName textureName, const vec2& position, const vec2& size, const vec2& origin, bool hidden);

    virtual ~UIElement() = default;

    string getID() const;

    void setPosition(float x, float y);

    void setSize(float x, float y);

    void setOrigin(float x, float y);

    // Adds all quad vertices to UI batch buffer
    virtual void generateVertices(const unique_ptr<UIBatch>& batch, const unique_ptr<UITextureAtlas>& textureAtlas) const;

    virtual void updateWindowSize(int width, int height);

    UITextureName textureName = UIT_NONE;
    vec2 position;
    vec2 size;
    vec2 origin { 0.0f, 0.0f };

    bool hidden = false;

    UI_RENDERPASS renderPass = UI_MAIN;

private:
    string id;
};


#endif //BLOCKGAME_UIELEMENT_H
