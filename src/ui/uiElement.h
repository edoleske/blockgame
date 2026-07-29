#ifndef BLOCKGAME_UIELEMENT_H
#define BLOCKGAME_UIELEMENT_H

#include "common.h"
#include "uiTextureAtlas.h"

class UIBatch;

enum UIRenderPass {
    UI_MAIN, UI_TEXT
};


struct UIElementConfig {
    // Mandatory string ID for identifying elements
    string id;
    // Name for texture in UI texture atlas
    UITextureName textureName = UIT_NONE;
    // Position of element's origin
    vec2 position = vec2(0.0f);
    // Size of element (keep same as pixel size of texture)
    vec2 size = vec2(0.0f);
    // Scale of element (multiplies size)
    float scale = 1.0f;
    // Origin of position (0.0f to 1.0f for x and y)
    vec2 origin = vec2(0.0f);
    // Hidden flag for skipping rendering element
    bool hidden = false;
    // Centers origin horizontally
    bool centerX = false;
    // Centers origin vertically
    bool centerY = false;
    // Controls which render pass elements is rendered in
    UIRenderPass renderPass = UI_MAIN;
};


class UIElement {
public:
    explicit UIElement(string id);

    explicit UIElement(UIElementConfig config);

    virtual ~UIElement() = default;

    string getID() const;

    void setPosition(float x, float y);

    void setSize(float x, float y);

    void setOrigin(float x, float y);

    // Adds all quad vertices to UI batch buffer
    virtual void generateVertices(
        const unique_ptr<UIBatch>& batch, const unique_ptr<UITextureAtlas>& textureAtlas) const;

    // Updates UI element when screen size changes
    virtual void updateWindowSize(int width, int height);

    UITextureName textureName;
    vec2 position;
    vec2 size;
    float scale;
    vec2 origin;
    bool centerX;
    bool centerY;
    bool hidden;
    UIRenderPass renderPass;

private:
    string id;
};


#endif //BLOCKGAME_UIELEMENT_H
