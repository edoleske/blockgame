#include "textBox.h"

#include <utility>

#include "ui/uiBatch.h"
#include "ui/uiRenderer.h"

TextBox::TextBox(string id, const shared_ptr<Font>& font)
    : UIElement(UIElementConfig{.id = std::move(id), .renderPass = UI_TEXT}), font(font) {}

TextBox::TextBox(UIElementConfig&& config, const shared_ptr<Font>& font) : UIElement(std::move(config)), font(font) {}

void TextBox::onRender(const UIRenderer& renderer, const UIRenderPass pass) const {
    if (pass != UI_TEXT || hidden) return;

    // origin starts as position adjusted for baseline and padding
    auto origin = vec2(Font::PADDING, font->getBaseline()) + position;

    for (auto& c : text) {
        auto packedChar = font->getPackedChar(c);
        auto quad = font->getQuad(c);
        renderer.getBatch()->insertQuad({
            .position = origin + vec2(packedChar.xoff, packedChar.yoff),
            .size = vec2(packedChar.x1 - packedChar.x0, packedChar.y1 - packedChar.y0),
            .uvMin = vec2(quad.s0, quad.t0),
            .uvMax = vec2(quad.s1, quad.t1)
        });

        origin.x += packedChar.xadvance;
    }
}
