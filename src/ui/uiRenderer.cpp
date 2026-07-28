#include "uiRenderer.h"

#include <format>

#include "font.h"
#include "elements/crosshair.h"
#include "elements/textBox.h"
#include "elements/toolbar.h"
#include "game/inputState.h"

UIRenderer::UIRenderer() {
    shader = make_unique<Shader>("../resources/shaders/ui.vert", "../resources/shaders/ui.frag");
    textureAtlas = make_unique<UITextureAtlas>("../resources/img/ui_texture.png");
    font = make_shared<Font>("../resources/font/ponderosa.ttf");

    elements.emplace_back(make_unique<Crosshair>());

    auto highlight = make_unique<UIElement>(UIElementConfig{
        .id = "toolbarHighlight", .textureName = UIT_HIGHLIGHT, .position = vec2(0.0f), .size = vec2(20.0f),
        .scale = 3.0f, .origin = vec2(0.0f, 1.0f), .hidden = false
    });
    elements.emplace_back(make_unique<Toolbar>(highlight.get()));
    elements.push_back(std::move(highlight));

    auto fpsCounter = make_unique<TextBox>("fpsCounter", font);
    fpsCounter->setPosition(1.0f, 1.0f);
    elements.push_back(std::move(fpsCounter));

    batch = make_unique<UIBatch>();
}

void UIRenderer::update(const float deltaTime, const Player& player) const {
    const auto input = InputState::getInstance();
    const auto toggleDebug = input->isPressed(InputEvent::TOGGLE_DEBUG);

    for (auto& element : elements) {
        if (element->getID() == "fpsCounter") {
            if (toggleDebug) element->hidden = !element->hidden;
            if (element->hidden) continue;

            if (const auto counter = dynamic_cast<TextBox*>(element.get()); counter != nullptr) {
                counter->text = std::format("FPS: {:.0f}", 1.0f / deltaTime);
            }

            break;
        }
        if (element->getID() == "toolbar") {
            const auto toolbar = dynamic_cast<Toolbar*>(element.get());
            if (toolbar == nullptr) continue;

            if (const auto selected = player.getInventory().getSelected();
                selected != toolbar->getHighlightPosition()) {
                toolbar->setHighlightPosition(selected);
            }
        }
    }
}

void UIRenderer::render() const {
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    shader->use();
    shader->setInteger("isText", 0);

    textureAtlas->getTexture()->bind();

    for (const auto& element : elements) {
        if (element->getID() == "fpsCounter") continue;
        if (element->hidden) continue;
        element->generateVertices(batch, textureAtlas);
    }
    batch->flush();

    font->getTexture()->bind();
    shader->setInteger("isText", 1);

    for (const auto& element : elements) {
        if (element->getID() != "fpsCounter") continue;
        if (element->hidden) continue;
        element->generateVertices(batch, textureAtlas);
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
