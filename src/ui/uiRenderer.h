#ifndef BLOCKGAME_UIRENDERER_H
#define BLOCKGAME_UIRENDERER_H

#include "font.h"
#include "uiElement.h"
#include "uiBatch.h"
#include "elements/textBox.h"
#include "game/inputState.h"
#include "gl/shader.h"
#include "gl/texture.h"

class UIRenderer {
public:
    UIRenderer();

    void update(float deltaTime, const InputState& input) const;

    void render() const;

    void updateWindowSize(int width, int height) const;

private:
    unique_ptr<Shader> shader;
    shared_ptr<Texture> crosshairTexture;
    shared_ptr<Font> font;

    vector<unique_ptr<UIElement>> elements = {};
    vector<unique_ptr<TextBox>> textElements = {};
    unique_ptr<UIBatch> batch;
};


#endif //BLOCKGAME_UIRENDERER_H
