#include "uiRenderer.h"

#include <format>

#include "font.h"
#include "elements/crosshair.h"
#include "elements/textBox.h"
#include "elements/toolbar.h"
#include "game/input.h"

UIRenderer::UIRenderer() {
    shader = make_unique<Shader>("../resources/shaders/ui.vert", "../resources/shaders/ui.frag");
    textureAtlas = make_unique<UITextureAtlas>("../resources/img/ui_texture.png");
    font = make_shared<Font>("../resources/font/ponderosa.ttf");

    elements.emplace_back(make_unique<Crosshair>());

    std::array<UIElement*, Inventory::MAX_HOTBAR_SLOTS> toolbarItemSprites{};
    for (int i = 0; i < Inventory::MAX_HOTBAR_SLOTS; i++) {
        toolbarItemSprites[i] = new UIElement(UIElementConfig{
            .id = "toolbarItemSprite" + std::to_string(i),
            .textureName = UIT_NONE,
            .size = vec2(20.0f),
            .scale = 3.0f,
            .origin = vec2(0.0f, 1.0f)
        });
    }

    auto highlight = make_unique<UIElement>(UIElementConfig{
        .id = "toolbarHighlight", .textureName = UIT_HIGHLIGHT, .position = vec2(0.0f), .size = vec2(20.0f),
        .scale = 3.0f, .origin = vec2(0.0f, 1.0f)
    });
    elements.emplace_back(make_unique<Toolbar>(highlight.get(), toolbarItemSprites));

    // Save toolbar's sub elements
    elements.push_back(std::move(highlight));
    for (const auto element : toolbarItemSprites) {
        elements.push_back(unique_ptr<UIElement>(element));
    }

    auto fpsCounter = make_unique<TextBox>("fpsCounter", font);
    fpsCounter->setPosition(1.0f, 1.0f);
    elements.push_back(std::move(fpsCounter));

    batch = make_unique<UIBatch>();
}

void UIRenderer::update(const float deltaTime, const Player& player) const {
    const auto input = Input::getInstance();
    const auto toggleDebug = input->isPressed(Input::Event::TOGGLE_DEBUG);

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

            toolbar->updateFromInventory(player.getInventory());
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
        if (element->hidden || element->textureName == UIT_NONE) continue;
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
