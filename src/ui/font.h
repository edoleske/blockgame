#ifndef BLOCKGAME_FONT_H
#define BLOCKGAME_FONT_H

#include "common.h"
#include "gl/texture.h"

#include "stb_truetype.h"


class Font {
public:
    explicit Font(const string& filename);

    stbtt_packedchar getPackedChar(char c) const;

    stbtt_aligned_quad getQuad(char c) const;

    Texture* getTexture() const;

private:
    unique_ptr<Texture> texture;

    static constexpr uint32_t ATLAS_WIDTH = 160;
    static constexpr uint32_t ATLAS_HEIGHT = 160;
    static constexpr uint32_t FIRST_CHAR = 32;
    static constexpr uint32_t TOTAL_CHARS = 96;
    static constexpr float FONT_SIZE = 16.0f;

    std::array<stbtt_packedchar, TOTAL_CHARS> chars = {};
    std::array<stbtt_aligned_quad, TOTAL_CHARS> quads = {};

    static inline int getCharIndex(char c);
};


#endif //BLOCKGAME_FONT_H
