#ifndef BLOCKGAME_UIRENDERER_H
#define BLOCKGAME_UIRENDERER_H

#include "font.h"
#include "uiElement.h"
#include "uiBatch.h"
#include "gl/shader.h"
#include "player/player.h"

class UIRenderer {
public:
    UIRenderer();

    void update(float deltaTime, const Player& player) const;

    void render() const;

    void updateWindowSize(int width, int height) const;

private:
    unique_ptr<Shader> shader;
    unique_ptr<UITextureAtlas> textureAtlas;
    shared_ptr<Font> font;

    vector<unique_ptr<UIElement>> elements = {};
    unique_ptr<UIBatch> batch;
};


#endif //BLOCKGAME_UIRENDERER_H
