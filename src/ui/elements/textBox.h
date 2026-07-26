#ifndef BLOCKGAME_TEXTBOX_H
#define BLOCKGAME_TEXTBOX_H
#include "ui/font.h"
#include "ui/uiElement.h"


class TextBox : public UIElement {
public:
    explicit TextBox(string id, const shared_ptr<Font>& font);

    explicit TextBox(UIElementConfig config, const shared_ptr<Font>& font);

    void generateVertices(const unique_ptr<UIBatch>& batch, const unique_ptr<UITextureAtlas>& _) const override;

    string text;
private:
    shared_ptr<Font> font;
};


#endif //BLOCKGAME_TEXTBOX_H
