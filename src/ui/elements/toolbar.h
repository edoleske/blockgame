#ifndef BLOCKGAME_TOOLBAR_H
#define BLOCKGAME_TOOLBAR_H

#include "common.h"
#include "game/player/inventory.h"
#include "ui/uiElement.h"


class Toolbar : public UIElement {
public:
    explicit Toolbar(UIElement* highlight, const std::array<UIElement*, Inventory::MAX_HOTBAR_SLOTS>& itemSprites);

    void updateFromInventory(const Inventory& inventory);

    void updateWindowSize(int width, int height) override;

    int getHighlightPosition() const;

    void setHighlightPosition(int index);

private:
    std::array<UIElement*, Inventory::MAX_HOTBAR_SLOTS> itemSprites{};
    UIElement* toolbarHighlight;
    int highlightPosition = 0;
};


#endif //BLOCKGAME_TOOLBAR_H
