#include "debugOverlay.h"

#include <format>

#include "utils/debug.h"

DebugOverlay::DebugOverlay(const shared_ptr<Font>& font) : UIElement(UIElementConfig{.id = "debugOverlay"}) {
    fpsCounter = make_unique<TextBox>("fpsCounter", font);
    fpsCounter->setPosition(1.0f, 1.0f);

    memCounter = make_unique<TextBox>("memCounter", font);
    memCounter->setPosition(1.0f, 16.0f);
}

void DebugOverlay::update(const float deltaTime) const {
    fpsCounter->text = std::format("FPS: {:.0f}", 1.0f / deltaTime);
    memCounter->text = std::format("Mem: {}", formatBytes(GetAllocatedMemory()));
}

void DebugOverlay::onRender(const UIRenderer& renderer, const UIRenderPass pass) const {
    if (hidden) return;
    fpsCounter->onRender(renderer, pass);
    memCounter->onRender(renderer, pass);
}
