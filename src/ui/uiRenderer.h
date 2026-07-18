#ifndef BLOCKGAME_UIRENDERER_H
#define BLOCKGAME_UIRENDERER_H

#include "font.h"
#include "uiElement.h"
#include "uiBatch.h"
#include "gl/shader.h"
#include "gl/texture.h"

class UIRenderer {
public:
    UIRenderer();

    void render() const;

    void updateWindowSize(int width, int height) const;

private:
    unique_ptr<Shader> shader;
    shared_ptr<Texture> crosshairTexture;
    shared_ptr<Font> font;

    vector<unique_ptr<UIElement>> elements = {};
    vector<unique_ptr<UIElement>> textElements = {};
    unique_ptr<UIBatch> batch;
};


#endif //BLOCKGAME_UIRENDERER_H
