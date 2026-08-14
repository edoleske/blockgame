#ifndef BLOCKGAME_UIRENDERER_H
#define BLOCKGAME_UIRENDERER_H

#include "font.h"
#include "uiBatch.h"
#include "uiElement.h"
#include "gl/shader.h"

class Game;


class UIRenderer {
public:
    UIRenderer();

    void update(const Game& game) const;

    void render() const;

    void updateWindowSize(int width, int height) const;

    UITextureAtlas* getTextureAtlas() const;

    UIBatch* getBatch() const;

private:
    unique_ptr<Shader> shader;
    unique_ptr<UITextureAtlas> textureAtlas;
    shared_ptr<Font> font;

    vector<unique_ptr<UIElement>> elements = {};
    unique_ptr<UIBatch> batch;
};


#endif //BLOCKGAME_UIRENDERER_H
