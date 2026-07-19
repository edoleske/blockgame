#include "uiRenderer.h"

#include <format>

#include "font.h"
#include "elements/crosshair.h"
#include "elements/textBox.h"

UIRenderer::UIRenderer() {
    shader = make_unique<Shader>("../resources/shaders/ui.vert", "../resources/shaders/ui.frag");
    uiTexture = make_shared<Texture>("../resources/img/ui_texture.png");
    font = make_shared<Font>("../resources/font/ponderosa.ttf");

    elements.emplace_back(make_unique<Crosshair>());
    elements.emplace_back(make_unique<UIElement>("toolbar", UIT_TOOLBAR, vec2(100.0f), vec2(200.0f, 20.0f), false));

    auto fpsCounter = make_unique<TextBox>("fpsCounter", font);
    fpsCounter->setPosition(1.0f, 1.0f);
    elements.push_back(std::move(fpsCounter));

    batch = make_unique<UIBatch>();
}

void UIRenderer::update(const float deltaTime, const InputState& input) const {
    const auto [current, previous] = input.getState(InputEvent::TOGGLE_DEBUG);
    const auto doFlip = current == true && !previous;

    for (auto& element : elements) {
        if (element->getID() == "fpsCounter") {
            if (doFlip) element->hidden = !element->hidden;
            if (element->hidden) continue;

            if (const auto counter = dynamic_cast<TextBox*>(element.get()); counter != nullptr) {
                counter->text = std::format("FPS: {:.0f}", 1.0f / deltaTime);
            }

            break;
        }
    }
}

void UIRenderer::render() const {
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    shader->use();
    shader->setInteger("isText", 0);

    uiTexture->bind();

    for (const auto& element : elements) {
        if (element->getID() == "fpsCounter") continue;
        if (element->hidden) continue;
        element->generateVertices(batch);
    }
    batch->flush();

    font->getTexture()->bind();
    shader->setInteger("isText", 1);

    for (const auto& element : elements) {
        if (element->getID() != "fpsCounter") continue;
        if (element->hidden) continue;
        element->generateVertices(batch);
    }
    batch->flush();

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
}

void UIRenderer::updateWindowSize(const int width, const int height) const {
    const mat4 projection = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, -1.0f, 1.0f);

    shader->use();
    shader->setMatrix4("projection", projection);

    for (const auto& element : elements) {
        element->updateWindowSize(width, height);
    }
}
