#ifndef BLOCKGAME_DEBUGOVERLAY_H
#define BLOCKGAME_DEBUGOVERLAY_H

#include "ui/uiElement.h"
#include "textBox.h"
#include "game/world/world.h"


class DebugOverlay : public UIElement {
public:
    explicit DebugOverlay(const shared_ptr<Font>& font);

    void update(float deltaTime, const World* world) const;

    void onRender(const UIRenderer& renderer, UIRenderPass pass) const override;

private:
    unique_ptr<TextBox> fpsCounter;
    unique_ptr<TextBox> memCounter;
    unique_ptr<TextBox> vertexCounter;
};


#endif //BLOCKGAME_DEBUGOVERLAY_H
