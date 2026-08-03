#include "textureArray.h"

#include "log.h"
#include "stb_image.h"

TextureArray::TextureArray(const GLenum slot) : slot(slot) {}

void TextureArray::allocate(const int resolution, const int layers) {
    bind();
    glTexStorage3D(target, 1, GL_RGBA8, resolution, resolution, layers);

    width = resolution;
    height = resolution;
    layerCount = layers;
}

void TextureArray::addLayer(const string& path) {
    if (layerIndex >= layerCount) {
        LOG_ERROR("Cannot add layer to texture array due to exceeding allocated layers");
        return;
    }

    int w, h, channels;
    const auto data = loadFile(path, w, h, channels);

    if (w != width || h != height) {
        LOG_ERROR("{} cannot be added due to mismatched resolution: {}x{}", path, w, h);
        return;
    }

    bind();
    glTexSubImage3D(
        target,
        0,
        0,
        0,
        layerIndex,
        width,
        height,
        1,
        getFormat(channels),
        GL_UNSIGNED_BYTE,
        data
    );
    layerIndex++;

    stbi_image_free(data);
}

void TextureArray::bind() const {
    Texture::bind(slot, target);
}

void TextureArray::unbind() const {
    Texture::unbind(slot, target);
}

void TextureArray::setParameter(GLenum param, GLint value) {
    Texture::setParameter(target, param, value);
}

int TextureArray::getLayerCount() const {
    return layerIndex;
}

int TextureArray::getWidth() const {
    return width;
}

int TextureArray::getHeight() const {
    return height;
}
