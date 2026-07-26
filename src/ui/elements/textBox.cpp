#include "textBox.h"

#include <utility>

#include "ui/uiBatch.h"

TextBox::TextBox(string id, const shared_ptr<Font>& font) : UIElement(std::move(id)), font(font) {
    renderPass = UI_TEXT;
}

TextBox::TextBox(string id, const vec2& position, const vec2& size, const vec2& origin, const bool hidden, const shared_ptr<Font>& font)
    : UIElement(std::move(id), UIT_NONE, position, size, origin, hidden), font(font) {
    renderPass = UI_TEXT;
}

void TextBox::generateVertices(const unique_ptr<UIBatch>& batch, const unique_ptr<UITextureAtlas>& _) const {
    // origin starts as position adjusted for baseline and padding
    auto origin = vec2(Font::PADDING, font->getBaseline()) + position;

    for (auto& c : text) {
        auto packedChar = font->getPackedChar(c);
        auto quad = font->getQuad(c);

        auto position = origin + vec2(packedChar.xoff, packedChar.yoff);
        auto size = vec2(packedChar.x1 - packedChar.x0, packedChar.y1 - packedChar.y0);
        auto uv0 = vec2(quad.s0, quad.t0);
        auto uv1 = vec2(quad.s1, quad.t1);
        batch->insertQuad(position, size, uv0, uv1);

        origin.x += packedChar.xadvance;
    }
}
