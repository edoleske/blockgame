#ifndef BLOCKGAME_TOOLBAR_H
#define BLOCKGAME_TOOLBAR_H

#include "common.h"
#include "itemSprite.h"
#include "game/player/inventory.h"
#include "ui/uiElement.h"


class Toolbar : public UIElement {
public:
    explicit Toolbar(const shared_ptr<Font>& font);

    void onRender(const UIRenderer& renderer, UIRenderPass pass) const override;

    void updateFromInventory(const Inventory& inventory);

    void updateWindowSize(int width, int height) override;

    int getHighlightPosition() const;

    void setHighlightPosition(int index);

private:
    std::array<unique_ptr<ItemSprite>, Inventory::MAX_HOTBAR_SLOTS> itemSprites{};
    unique_ptr<UIElement> highlight;
    int highlightPosition = 0;
};


#endif //BLOCKGAME_TOOLBAR_H
