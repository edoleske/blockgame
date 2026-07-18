#include "textBox.h"

#include <utility>

#include "ui/uiBatch.h"

TextBox::TextBox(string text, const shared_ptr<Font>& font) : text(std::move(text)), font(font) {}

void TextBox::generateVertices(const unique_ptr<UIBatch>& batch) const {
    vec2 origin = vec2(0.0f, 16.0f);
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
