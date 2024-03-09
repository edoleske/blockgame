#ifndef BLOCKGAME_BLOCKTEXTURE_H
#define BLOCKGAME_BLOCKTEXTURE_H


#include "common.h"
#include "gl/texture.h"

enum class BlockTextureName {
    DIRT,
    STONE
};

class BlockTexture {
public:
    BlockTexture();

    Texture* getTexture() const;

    const vec2& getTextureCoordinates(BlockTextureName name) const;

    const vec2& getOffset() const;

private:
    unique_ptr<Texture> texture;
    map<BlockTextureName, vec2> textureCoordinateMap;
    vec2 offset{};

    static const int RESOLUTION = 16;
};


#endif //BLOCKGAME_BLOCKTEXTURE_H
