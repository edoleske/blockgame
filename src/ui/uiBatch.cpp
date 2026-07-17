#include "uiBatch.h"

UIBatch::UIBatch() {
    quadVAO.bind();
    quadVBO.bind();
    quadVBO.vertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, nullptr);
    quadVBO.vertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(2 * sizeof(float)));
    quadVBO.bufferData(sizeof(float) * 16 * MAX_SPRITES, vertexBuffer.data(), GL_DYNAMIC_DRAW);
    precalculateElementBuffer(quadEBO);
    VertexArray::unbind();
}

void UIBatch::insertQuad(vec2 position, vec2 size) {
    if (count >= MAX_SPRITES) flush();

    for (auto i = 0; i < std::size(QUAD_VERTICES); i++) {
        auto vertex = QUAD_VERTICES[i];
        vertexBuffer[i + 4 * count] = {(vertex.position * size) + position, vertex.uv};
    }
    count++;
}

void UIBatch::flush() {
    submitBatch();
    count = 0;
}

void UIBatch::submitBatch() const {
    if (count <= 0) return;

    quadVBO.bind();
    glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * count * sizeof(UIVertex), vertexBuffer.data());

    quadVAO.bind();
    glDrawElements(GL_TRIANGLES, count * 6, GL_UNSIGNED_INT, nullptr);

    VertexArray::unbind();
}

void UIBatch::precalculateElementBuffer(ElementBuffer& ebo) {
    ebo.bind();

    GLuint indices[6 * MAX_SPRITES];
    GLuint offset = 0;
    for (auto i = 0; i < 6 * MAX_SPRITES; i += 6) {
        indices[i + 0] = offset + 0;
        indices[i + 1] = offset + 1;
        indices[i + 2] = offset + 2;
        indices[i + 3] = offset + 2;
        indices[i + 4] = offset + 3;
        indices[i + 5] = offset + 0;
        offset += 4;
    }

    ebo.bufferData(sizeof(indices), indices);
}