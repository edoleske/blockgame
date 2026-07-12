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

    void updateWindowSize(int width, int height) const;

private:
    unique_ptr<Shader> shader;
    shared_ptr<Texture> crosshairTexture;

    vector<unique_ptr<UIElement>> elements = {};

    static constexpr size_t MAX_SPRITES = 100;
    inline static UIVertex QUAD_VERTICES[4] = {
        {{1.0f, 0.0f}, {1.0f, 0.0f}}, // TOP RIGHT
        {{1.0f, 1.0f}, {1.0f, 1.0f}}, // BOTTOM RIGHT
        {{0.0f, 1.0f}, {0.0f, 1.0f}}, // BOTTOM LEFT
        {{0.0f, 0.0f}, {0.0f, 0.0f}}, // TOP LEFT
    };

    VertexBuffer quadVBO;
    VertexArray quadVAO;
    ElementBuffer quadEBO;
    std::array<UIVertex, MAX_SPRITES * 4> vertexBuffer = {};

    void submitBatch(size_t count) const;

    static void precalculateElementBuffer(ElementBuffer& ebo);
};


#endif //BLOCKGAME_UIMANAGER_H
