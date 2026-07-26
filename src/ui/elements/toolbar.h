#ifndef BLOCKGAME_TOOLBAR_H
#define BLOCKGAME_TOOLBAR_H
#include "ui/uiElement.h"


class Toolbar : public UIElement {
public:
    explicit Toolbar(UIElement* highlight);

    void updateWindowSize(int width, int height) override;

    int getHighlightPosition() const;

    void setHighlightPosition(int index);

private:
    UIElement* toolbarHighlight;
    int highlightPosition = 0;
};


#endif //BLOCKGAME_TOOLBAR_H
