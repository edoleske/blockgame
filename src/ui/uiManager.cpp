#include "uiManager.h"

UIManager::UIManager() {
    shader = make_unique<Shader>("../resources/shaders/ui.vert", "../resources/shaders/ui.frag");
    crosshairTexture = make_shared<Texture>("../resources/img/crosshair.png");

    quadVAO.bind();
    quadVBO.bind();
    quadVBO.vertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*) nullptr);
    quadVBO.vertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(2 * sizeof(float)));
    quadVBO.bufferData(sizeof(quadData), quadData, GL_STATIC_DRAW);
    VertexArray::unbind();

    elements = vector<UIElement>();
    elements.emplace_back(crosshairTexture, vec2(0.0f), vec2(16.0f));
}

void UIManager::render(int width, int height) {
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    shader->use();

    mat4 projection = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, -1.0f, 1.0f);
    shader->setMatrix4("projection", projection);

    quadVAO.bind();

    for (UIElement& element : elements) {
        shader->setMatrix4("model", element.getModel());
        element.getTexture()->bind();
        shader->setInteger("uTexWidth", element.getTexture()->getWidth());
        shader->setInteger("uTexHeight", element.getTexture()->getHeight());
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    VertexArray::unbind();

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
}

void UIManager::updateWindowSize(int width, int height) {
    for (UIElement& element : elements) {
        element.updateWindowSize(width, height);
    }
}


