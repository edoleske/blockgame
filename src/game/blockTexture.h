#ifndef BLOCKGAME_BLOCKTEXTURE_H
#define BLOCKGAME_BLOCKTEXTURE_H


#include "common.h"
#include "gl/texture.h"

enum class BlockTextureName {
    NONE,
    DIRT,
    GRASS_SIDE,
    GRASS,
    BEDROCK,
    STONE
};

class BlockTexture {
public:
    BlockTexture();

    Texture* getTexture() const;

    const u16vec2& getTextureCoordinates(BlockTextureName name) const;

    static const unsigned short RESOLUTION = 16;

private:
    unique_ptr<Texture> texture;
    map<BlockTextureName, u16vec2> textureCoordinateMap;
    vec2 offset{};
};


#endif //BLOCKGAME_BLOCKTEXTURE_H
