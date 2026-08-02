#include "textureArray.h"

#include "log.h"
#include "stb_image.h"

TextureArray::TextureArray() {
    glGenTextures(1, &texture);
    bind();

    glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

TextureArray::~TextureArray() {
    glDeleteTextures(1, &texture);
}

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
    unsigned char* data = stbi_load(path.c_str(), &w, &h, &channels, 0);
    if (!data) {
        LOG_ERROR("Failed to load texture {}", path);
        return;
    }

    if (w != width || h != height) {
        LOG_ERROR("{} cannot be added due to mismatched resolution: {}x{}", path, w, h);
        return;
    }

    auto format = channels == 4 ? GL_RGBA : GL_RGB;
    bind();
    glTexSubImage3D(target, 0, 0, 0, layerIndex, width, height, 1, format, GL_UNSIGNED_BYTE, data);
    layerIndex++;

    stbi_image_free(data);
}

void TextureArray::bind() const {
    glBindTexture(target, texture);
}

void TextureArray::unbind() {
    glBindTexture(target, 0);
}

GLuint TextureArray::getTexture() const {
    return texture;
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
