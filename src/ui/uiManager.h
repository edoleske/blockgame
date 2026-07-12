#ifndef BLOCKGAME_UIMANAGER_H
#define BLOCKGAME_UIMANAGER_H

#include "uiElement.h"
#include "gl/elementBuffer.h"
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
        1.0f, 1.0f, 1.0f, 1.0f, // BOTTOM RIGHT
        0.0f, 1.0f, 0.0f, 1.0f, // BOTTOM LEFT
        0.0f, 0.0f, 0.0f, 0.0f, // TOP LEFT
    };
    ElementBuffer quadEBO;
    GLuint quadIndices[6] = {
        0, 1, 3,
        1, 2, 3
    };
};


#endif //BLOCKGAME_UIMANAGER_H
