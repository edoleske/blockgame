#ifndef BLOCKGAME_UIBATCH_H
#define BLOCKGAME_UIBATCH_H

#include "common.h"
#include "gl/elementBuffer.h"
#include "gl/vertexArray.h"
#include "gl/vertexBuffer.h"


struct UIVertex {
    vec2 position;
    vec2 uv;
    float layer;
};

struct QuadConfig {
    vec2 position = vec2(0.0f);
    vec2 size = vec2(1.0f);
    vec2 uvMin = vec2(0.0f);
    vec2 uvMax = vec2(1.0f);
    float layer = 0.0f;
};


class UIBatch {
public:
    UIBatch();

    static constexpr size_t MAX_SPRITES = 100;

    void insertQuad(const QuadConfig& config);

    void flush();
private:
    size_t count = 0;

    VertexBuffer quadVBO;
    VertexArray quadVAO;
    ElementBuffer quadEBO;
    std::array<UIVertex, MAX_SPRITES * 4> vertexBuffer = {};

    inline static UIVertex QUAD_VERTICES[4] = {
        {.position = {1.0f, 0.0f}, .uv = {1.0f, 0.0f}}, // TOP RIGHT
        {.position = {1.0f, 1.0f}, .uv = {1.0f, 1.0f}}, // BOTTOM RIGHT
        {.position = {0.0f, 1.0f}, .uv = {0.0f, 1.0f}}, // BOTTOM LEFT
        {.position = {0.0f, 0.0f}, .uv = {0.0f, 0.0f}}, // TOP LEFT
    };

    void submitBatch() const;

    static void precalculateElementBuffer(ElementBuffer& ebo);
};


#endif //BLOCKGAME_UIBATCH_H
