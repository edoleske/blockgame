#ifndef BLOCKGAME_UIMANAGER_H
#define BLOCKGAME_UIMANAGER_H

#include "uiElement.h"
#include "gl/shader.h"
#include "gl/texture.h"
#include "gl/vertexArray.h"
#include "gl/vertexBuffer.h"


class UIManager {
public:
    UIManager();

    void render(int width, int height);

    void updateWindowSize(int width, int height);
private:
    unique_ptr<Shader> shader;
    shared_ptr<Texture> crosshairTexture;

    vector<UIElement> elements;

    VertexBuffer quadVBO;
    VertexArray quadVAO;
    float quadData[16] = {
        1.0f, 0.0f, 1.0f, 0.0f, // TOP RIGHT
        0.0f, 0.0f, 0.0f, 0.0f, // TOP LEFT
        1.0f, 1.0f, 1.0f, 1.0f, // BOTTOM RIGHT
        0.0f, 1.0f, 0.0f, 1.0f, // BOTTOM LEFT
    };
};


#endif //BLOCKGAME_UIMANAGER_H
