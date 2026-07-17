#ifndef BLOCKGAME_UIBATCH_H
#define BLOCKGAME_UIBATCH_H

#include "common.h"
#include "uiElement.h"
#include "gl/elementBuffer.h"
#include "gl/vertexArray.h"
#include "gl/vertexBuffer.h"


class UIBatch {
public:
    UIBatch();

    static constexpr size_t MAX_SPRITES = 100;

    void insertQuad(vec2 position, vec2 size);

    void flush();
private:
    size_t count = 0;

    VertexBuffer quadVBO;
    VertexArray quadVAO;
    ElementBuffer quadEBO;
    std::array<UIVertex, MAX_SPRITES * 4> vertexBuffer = {};

    inline static UIVertex QUAD_VERTICES[4] = {
        {{1.0f, 0.0f}, {1.0f, 0.0f}}, // TOP RIGHT
        {{1.0f, 1.0f}, {1.0f, 1.0f}}, // BOTTOM RIGHT
        {{0.0f, 1.0f}, {0.0f, 1.0f}}, // BOTTOM LEFT
        {{0.0f, 0.0f}, {0.0f, 0.0f}}, // TOP LEFT
    };

    void submitBatch() const;

    static void precalculateElementBuffer(ElementBuffer& ebo);
};


#endif //BLOCKGAME_UIBATCH_H
