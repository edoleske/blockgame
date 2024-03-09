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
            textureCoordinateMap[name] = vec2(
                    static_cast<float>(x) * offset.x,
                    static_cast<float>(y) * offset.y);
        }
    }
}

Texture* BlockTexture::getTexture() const {
    return texture.get();
}

const vec2& BlockTexture::getTextureCoordinates(BlockTextureName name) const {
    auto coordinates = textureCoordinateMap.find(name);
    if (coordinates != textureCoordinateMap.end()) {
        return coordinates->second;
    }
    return {0.0f, 1.0f};
}

const vec2& BlockTexture::getOffset() const {
    return offset;
}
