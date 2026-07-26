#ifndef BLOCKGAME_UITEXTUREATLAS_H
#define BLOCKGAME_UITEXTUREATLAS_H

#include "common.h"
#include "gl/texture.h"

enum UITextureName {
    UIT_NONE, UIT_TOOLBAR, UIT_CROSSHAIR, UIT_HIGHLIGHT, UIT_PLACEHOLDER
};

// Pos (x, y) Size (x, y)
static constexpr std::array UI_TEXTURES_INFO = {
    ivec4(0, 0, 0, 0),
    ivec4(0, 0, 200, 20),
    ivec4(2, 22, 16, 16),
    ivec4(20, 20, 20, 20),
    ivec4(40, 20, 20, 20)
};


class UITextureAtlas {
public:
    explicit UITextureAtlas(const string& filename);

    Texture* getTexture() const;

    vec4 getUV(UITextureName textureName) const;
private:
    unique_ptr<Texture> texture;

    map<UITextureName, vec4> uvMap;
    void calculateTextureCoordinates();
};


#endif //BLOCKGAME_UITEXTUREATLAS_H
