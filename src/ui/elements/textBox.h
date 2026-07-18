#ifndef BLOCKGAME_TEXTBOX_H
#define BLOCKGAME_TEXTBOX_H
#include "ui/font.h"
#include "ui/uiElement.h"


class TextBox : public UIElement {
public:
    explicit TextBox(string text, const shared_ptr<Font>& font);

    void generateVertices(const unique_ptr<UIBatch>& batch) const override;
private:
    string text;
    shared_ptr<Font> font;
};


#endif //BLOCKGAME_TEXTBOX_H
