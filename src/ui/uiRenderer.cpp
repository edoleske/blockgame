#include "uiRenderer.h"

#include <format>

#include "font.h"
#include "elements/crosshair.h"
#include "elements/debugOverlay.h"
#include "elements/toolbar.h"
#include "game/game.h"
#include "game/input.h"

UIRenderer::UIRenderer() {
    shader = make_unique<Shader>("../resources/shaders/ui.vert", "../resources/shaders/ui.frag");
    textureAtlas = make_unique<UITextureAtlas>("../resources/img/ui_texture.png", GL_TEXTURE1);
    shader->setInteger("uAtlas", 1);
    font = make_shared<Font>("../resources/font/ponderosa.ttf", GL_TEXTURE2);
    shader->setInteger("uFont", 2);

    elements.emplace_back(make_unique<Crosshair>());
    elements.emplace_back(make_unique<Toolbar>(font));
    elements.emplace_back(make_unique<DebugOverlay>(font));

    batch = make_unique<UIBatch>();
}

void UIRenderer::update(const Game& game) const {
    const auto input = Input::getInstance();
    const auto toggleDebug = input->isPressed(Input::Event::TOGGLE_DEBUG);

    for (auto& element : elements) {
        if (element->getID() == "debugOverlay") {
            if (toggleDebug) element->hidden = !element->hidden;
            if (element->hidden) continue;

            if (const auto overlay = dynamic_cast<DebugOverlay*>(element.get()); overlay != nullptr) {
                overlay->update(game.getDeltaTime(), game.getWorld());
            }

            continue;
        }
        if (element->getID() == "toolbar") {
            const auto toolbar = dynamic_cast<Toolbar*>(element.get());
            if (toolbar == nullptr) continue;

            toolbar->updateFromInventory(game.getPlayer().getInventory());
        }
    }
}

void UIRenderer::render() const {
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    shader->use();

    // Main UI render pass with UI texture atlas
    textureAtlas->getTexture()->bind();
    shader->setInteger("uMode", 0);
    for (const auto& element : elements) {
        element->onRender(*this, UI_MAIN);
    }
    batch->flush();

    // Block and item render pass that uses main texture array painted over UI
    shader->setInteger("uMode", 1);
    for (const auto& element : elements) {
        element->onRender(*this, UI_ITEM);
    }
    batch->flush();

    // Text UI render pass that uses bitmap font
    font->getTexture()->bind();
    shader->setInteger("uMode", 2);
    for (const auto& element : elements) {
        element->onRender(*this, UI_TEXT);
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

UITextureAtlas* UIRenderer::getTextureAtlas() const {
    return textureAtlas.get();
}

UIBatch* UIRenderer::getBatch() const {
    return batch.get();
}
