#include "uiManager.h"

#include "crosshair.h"

UIManager::UIManager() {
    shader = make_unique<Shader>("../resources/shaders/ui.vert", "../resources/shaders/ui.frag");
    crosshairTexture = make_shared<Texture>("../resources/img/crosshair.png");

    quadVAO.bind();
    quadVBO.bind();
    quadVBO.vertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, nullptr);
    quadVBO.vertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(2 * sizeof(float)));
    quadVBO.bufferData(sizeof(float) * 16 * MAX_SPRITES, vertexBuffer.data(), GL_DYNAMIC_DRAW);
    precalculateElementBuffer(quadEBO);
    VertexArray::unbind();

    elements.emplace_back(make_unique<Crosshair>(vec2(0.0f), vec2(16.0f)));
    elements.emplace_back(make_unique<UIElement>(vec2(100.0f), vec2(16.0f)));
}

void UIManager::render(int width, int height) {
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    shader->use();

    mat4 projection = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, -1.0f, 1.0f);
    shader->setMatrix4("projection", projection);

    crosshairTexture->bind();
    shader->setInteger("uTexWidth", 16);
    shader->setInteger("uTexHeight", 16);

    size_t count = 0;
    for (const auto& element : elements) {
        if (count >= MAX_SPRITES) {
            submitBatch(count);
            count = 0;
        }

        for (auto i = 0; i < std::size(QUAD_VERTICES); i++) {
            vertexBuffer[i + 4 * count] = element->transformVertex(QUAD_VERTICES[i]);
        }
        count++;
    }
    submitBatch(elements.size());

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
}

void UIManager::updateWindowSize(int width, int height) const {
    for (const auto& element : elements) {
        element->updateWindowSize(width, height);
    }
}

void UIManager::submitBatch(size_t count) const {
    if (count <= 0) return;

    quadVBO.bind();
    glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * count * sizeof(UIVertex), vertexBuffer.data());

    quadVAO.bind();
    glDrawElements(GL_TRIANGLES, count * 6, GL_UNSIGNED_INT, nullptr);

    VertexArray::unbind();
}

void UIManager::precalculateElementBuffer(ElementBuffer& ebo) {
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
