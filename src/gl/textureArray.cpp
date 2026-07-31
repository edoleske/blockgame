#include "textureArray.h"
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
    if (auto error = glGetError()) {
        std::cerr << "Failed to generate texture array storage: " << error << std::endl;
        return;
    }

    width = resolution;
    height = resolution;
    layerCount = layers;
}

void TextureArray::addLayer(const string& path) {
    if (layerIndex >= layerCount) {
        std::cerr << "Cannot add layer to texture array due to exceeding allocated layers" << std::endl;
        return;
    }

    int w, h, channels;
    unsigned char* data = stbi_load(path.c_str(), &w, &h, &channels, 0);
    if (!data) {
        std::cerr << "Failed to load texture " << path << std::endl;
        return;
    }

    if (w != width || h != height) {
        std::cerr << path << " resolution is invalid" << std::endl;
        return;
    }

    auto format = channels == 4 ? GL_RGBA : GL_RGB;
    bind();
    glTexSubImage3D(target, 0, 0, 0, layerIndex, width, height, 1, format, GL_UNSIGNED_BYTE, data);
    if (auto error = glGetError()) {
        std::cerr << "Failed to generate texture array subimage " << layerIndex << ": " << error << std::endl;
    }
    layerIndex++;

    stbi_image_free(data);
}

void TextureArray::bind() const {
    glBindTexture(target, texture);
    if (auto error = glGetError()) {
        std::cerr << "Failed to bind texture: " << error << std::endl;
    }
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
