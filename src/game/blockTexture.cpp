#include "blockTexture.h"

BlockTexture::BlockTexture() {
    texture = make_unique<Texture>("../resources/img/test_texture.png");

    int width = texture->getWidth();
    int height = texture->getHeight();

    offset = vec2(RESOLUTION / static_cast<float>(width), RESOLUTION / static_cast<float>(height));
    int cols = width / RESOLUTION;
    int rows = height / RESOLUTION;

    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            auto name = static_cast<BlockTextureName>(y * cols + x);
            textureCoordinateMap[name] = u16vec2(x * RESOLUTION,y * RESOLUTION);
        }
    }
}

Texture* BlockTexture::getTexture() const {
    return texture.get();
}

const u16vec2& BlockTexture::getTextureCoordinates(BlockTextureName name) const {
    auto coordinates = textureCoordinateMap.find(name);
    if (coordinates != textureCoordinateMap.end()) {
        return coordinates->second;
    }
    return textureCoordinateMap.begin()->second;
}
