#include "uiTextureAtlas.h"

UITextureAtlas::UITextureAtlas(const string& filename) {
    texture = make_unique<Texture>(filename);
    calculateTextureCoordinates();
}

Texture* UITextureAtlas::getTexture() const {
    return texture.get();
}

vec4 UITextureAtlas::getUV(const UITextureName textureName) const {
    return uvMap.at(textureName);
}

void UITextureAtlas::calculateTextureCoordinates() {
    uvMap = map<UITextureName, vec4>();
    const auto textureWidth = texture->getWidth();
    const auto textureHeight = texture->getHeight();

    for (size_t index = 0; auto const& info : UI_TEXTURES_INFO) {
        auto textureName = static_cast<UITextureName>(index);
        index++;

        const int w = info.z;
        const int h = info.w;

        uvMap[textureName] = vec4(
            static_cast<float>(info.x) / textureWidth,
            static_cast<float>(info.y) / textureHeight,
            static_cast<float>(info.x + w) / textureWidth,
            static_cast<float>(info.y + h) / textureHeight
        );
    }
}
