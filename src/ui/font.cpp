#include "font.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

Font::Font(const string& filename) {
    std::ifstream ifs(filename, std::ios::binary | std::ios::ate);
    if (!ifs.is_open()) {
        throw std::runtime_error("Could not open font file");
    }

    std::streamsize size = ifs.tellg();
    ifs.seekg(0, std::ios::beg);

    vector<unsigned char> ttfBuffer(size);
    ifs.read(reinterpret_cast<char*>(ttfBuffer.data()), size);

    vector<unsigned char> atlasBuffer(ATLAS_WIDTH * ATLAS_HEIGHT, 0);
    stbtt_pack_context packContext;
    if (!stbtt_PackBegin(&packContext, atlasBuffer.data(), ATLAS_WIDTH, ATLAS_HEIGHT, 0, 1, nullptr)) {
        throw std::runtime_error("Could not pack font data");
    }

    stbtt_PackFontRange(&packContext, ttfBuffer.data(), 0, FONT_SIZE, FIRST_CHAR, TOTAL_CHARS, chars.data());
    stbtt_PackEnd(&packContext);

    for (int i = 0; i < TOTAL_CHARS; i++) {
        float x, y;
        stbtt_GetPackedQuad(chars.data(), ATLAS_WIDTH, ATLAS_HEIGHT, i, &x, &y, &quads[i], 0);
    }

    texture = make_unique<Texture>(atlasBuffer, ATLAS_WIDTH, ATLAS_HEIGHT);
}

stbtt_packedchar Font::getPackedChar(const char c) const {
    return chars[getCharIndex(c)];
}

stbtt_aligned_quad Font::getQuad(const char c) const {
    return quads[getCharIndex(c)];
}

Texture* Font::getTexture() const {
    return texture.get();
}

int Font::getCharIndex(const char c) {
    const int32_t index = static_cast<int32_t>(c) - FIRST_CHAR;
    return index >= 0 && index < TOTAL_CHARS ? index : TOTAL_CHARS - 1;
}
