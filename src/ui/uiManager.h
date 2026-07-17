#ifndef BLOCKGAME_UIMANAGER_H
#define BLOCKGAME_UIMANAGER_H

#include "uiElement.h"
#include "uiBatch.h"
#include "gl/shader.h"
#include "gl/texture.h"


class UIManager {
public:
    UIManager();

    void render(int width, int height) const;

    void updateWindowSize(int width, int height) const;

private:
    unique_ptr<Shader> shader;
    shared_ptr<Texture> crosshairTexture;

    vector<unique_ptr<UIElement>> elements = {};
    unique_ptr<UIBatch> batch;
};


#endif //BLOCKGAME_UIMANAGER_H
