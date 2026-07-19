#ifndef BLOCKGAME_UIELEMENT_H
#define BLOCKGAME_UIELEMENT_H

#include "common.h"


class UIBatch;

struct UIVertex {
    vec2 position;
    vec2 uv;
};

enum UITextureName {
    UIT_NONE, UIT_TOOLBAR, UIT_CROSSHAIR, UIT_HIGHLIGHT, UIT_PLACEHOLDER
};

struct UITextureInfo {
    float x, y, w, h;
};

// TODO: Use the actual texture properties
constexpr int UI_TEXTURE_WIDTH = 200;
constexpr int UI_TEXTURE_HEIGHT = 40;

inline std::map<UITextureName, UITextureInfo> uiTextureInfoMap = {
    {UIT_NONE, {0, 0, 0, 0,}},
    {UIT_TOOLBAR,  {0, 0, 200, 20}},
    {UIT_CROSSHAIR, {2, 22, 16, 16}},
    {UIT_HIGHLIGHT, {20, 20, 20, 20}},
    {UIT_PLACEHOLDER, {40, 20, 20, 20}},
};


class UIElement {
public:
    explicit UIElement(string id);

    explicit UIElement(string id, UITextureName textureName, const vec2& position, const vec2& size, bool hidden);

    virtual ~UIElement() = default;

    string getID() const;

    void setPosition(float x, float y);

    void setSize(float x, float y);

    // Adds all quad vertices to UI batch buffer
    virtual void generateVertices(const unique_ptr<UIBatch>& batch) const;

    virtual void updateWindowSize(int width, int height);

    UITextureName textureName = UIT_NONE;
    vec2 position;
    vec2 size;

    bool hidden = false;

private:
    string id;
};


#endif //BLOCKGAME_UIELEMENT_H
