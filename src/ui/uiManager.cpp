#include "uiManager.h"

#include "elements/crosshair.h"

UIManager::UIManager() {
    shader = make_unique<Shader>("../resources/shaders/ui.vert", "../resources/shaders/ui.frag");
    crosshairTexture = make_shared<Texture>("../resources/img/crosshair.png");

    elements.emplace_back(make_unique<Crosshair>(vec2(0.0f), vec2(16.0f)));
    elements.emplace_back(make_unique<UIElement>(vec2(100.0f), vec2(16.0f)));

    batch = make_unique<UIBatch>();
}

void UIManager::render(int width, int height) const {
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    shader->use();

    mat4 projection = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, -1.0f, 1.0f);
    shader->setMatrix4("projection", projection);

    crosshairTexture->bind();
    shader->setInteger("uTexWidth", 16);
    shader->setInteger("uTexHeight", 16);

    for (const auto& element : elements) {
        element->generateVertices(batch);
    }
    batch->flush();

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
}

void UIManager::updateWindowSize(int width, int height) const {
    for (const auto& element : elements) {
        element->updateWindowSize(width, height);
    }
}
