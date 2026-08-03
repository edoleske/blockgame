#ifndef BLOCKGAME_FONT_H
#define BLOCKGAME_FONT_H

#include "common.h"
#include "gl/texture2D.h"

#include "stb_truetype.h"


class Font {
public:
    explicit Font(const string& filename);

    explicit Font(const string& filename, GLint slot);

    float getBaseline() const;

    stbtt_packedchar getPackedChar(char c) const;

    stbtt_aligned_quad getQuad(char c) const;

    const Texture2D* getTexture() const;

    static constexpr uint32_t ATLAS_WIDTH = 160;
    static constexpr uint32_t ATLAS_HEIGHT = 160;
    static constexpr uint32_t FIRST_CHAR = 32;
    static constexpr uint32_t TOTAL_CHARS = 96;
    static constexpr int PADDING = 2;
    static constexpr float FONT_SIZE = 16.0f;
private:
    Texture2D texture;

    stbtt_fontinfo fontInfo = {};
    int ascent = 0, descent = 0, lineGap = 0;

    std::array<stbtt_packedchar, TOTAL_CHARS> chars = {};
    std::array<stbtt_aligned_quad, TOTAL_CHARS> quads = {};

    void loadFontFile(const string& filename);

    static inline int getCharIndex(char c);

    inline float getScale() const;
};


#endif //BLOCKGAME_FONT_H
