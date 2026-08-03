#include "uiTextureAtlas.h"

UITextureAtlas::UITextureAtlas(const string& filename) {
    initializeTexture(filename);
}

UITextureAtlas::UITextureAtlas(const string& filename, const GLint slot): texture(slot) {
    initializeTexture(filename);
}

const Texture2D* UITextureAtlas::getTexture() const {
    return &texture;
}

vec4 UITextureAtlas::getUV(const UITextureName textureName) const {
    return uvMap.at(textureName);
}

void UITextureAtlas::initializeTexture(const string& filename) {
    texture.load(filename);

    // Set parameters on texture we just bound and uploaded data to
    Texture2D::setParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
    Texture2D::setParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
    Texture2D::setParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    Texture2D::setParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    calculateTextureCoordinates();
}

void UITextureAtlas::calculateTextureCoordinates() {
    uvMap = map<UITextureName, vec4>();
    const auto textureWidth = texture.getWidth();
    const auto textureHeight = texture.getHeight();

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
