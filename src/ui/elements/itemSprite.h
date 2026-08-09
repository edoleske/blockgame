#ifndef BLOCKGAME_ITEMSPRITE_H
#define BLOCKGAME_ITEMSPRITE_H

#include "textBox.h"
#include "game/item/item.h"
#include "ui/uiElement.h"


class ItemStack;

class ItemSprite : public UIElement {
public:
    explicit ItemSprite(UIElementConfig config);

    void onRender(const UIRenderer& renderer, UIRenderPass pass) const override;

    void updatePosition(vec2 newPos);

    void setItem(Item* newItem);

    int amount = 0;
private:
    Item* item = nullptr;
    unique_ptr<TextBox> amountLabel;
};


#endif //BLOCKGAME_ITEMSPRITE_H
