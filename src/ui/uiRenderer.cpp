#include "uiRenderer.h"

#include <format>

#include "font.h"
#include "elements/crosshair.h"
#include "elements/textBox.h"

UIRenderer::UIRenderer() {
    shader = make_unique<Shader>("../resources/shaders/ui.vert", "../resources/shaders/ui.frag");
    crosshairTexture = make_shared<Texture>("../resources/img/crosshair.png");
    font = make_shared<Font>("../resources/font/ponderosa.ttf");

    elements.emplace_back(make_unique<Crosshair>(vec2(0.0f), vec2(16.0f)));
    elements.emplace_back(make_unique<UIElement>(vec2(100.0f), vec2(16.0f)));

    textElements.emplace_back(make_unique<TextBox>("FPS: ", font));
    textElements[0]->setPosition(1.0f, 1.0f);

    batch = make_unique<UIBatch>();

}

void UIRenderer::update(const float deltaTime, const InputState& input) const {
    const auto toggleDebug = input.getState(InputEvent::TOGGLE_DEBUG);
    if (toggleDebug.current == true && !toggleDebug.previous) {
        textElements[0]->hidden = !textElements[0]->hidden;
    }

    textElements[0]->text = std::format("FPS: {:.0f}", 1.0f / deltaTime);
}

void UIRenderer::render() const {
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    shader->use();
    shader->setInteger("isText", 0);

    crosshairTexture->bind();

    for (const auto& element : elements) {
        if (element->hidden) continue;
        element->generateVertices(batch);
    }
    batch->flush();

    font->getTexture()->bind();
    shader->setInteger("isText", 1);

    for (const auto& element : textElements) {
        if (element->hidden) continue;
        element->generateVertices(batch);
    }
    batch->flush();

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
}

void UIRenderer::updateWindowSize(int width, int height) const {
    mat4 projection = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, -1.0f, 1.0f);

    shader->use();
    shader->setMatrix4("projection", projection);

    for (const auto& element : elements) {
        element->updateWindowSize(width, height);
    }
}
