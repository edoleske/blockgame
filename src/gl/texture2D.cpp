#include "texture2D.h"

#include "stb_image.h"

Texture2D::Texture2D(const GLenum slot) : slot(slot) {}

void Texture2D::load(const string& path) {
    const auto data = loadFile(path, width, height, channels);
    if (data == nullptr) {
        return;
    }

    uploadTextureData(data);
    stbi_image_free(data);
}

void Texture2D::load(const unsigned char* data, GLsizei width, GLsizei height, int channels) {
    this->width = width;
    this->height = height;
    this->channels = channels;
    uploadTextureData(data);
}

void Texture2D::bind() const {
    Texture::bind(slot, target);
}

void Texture2D::unbind() const {
    Texture::unbind(slot, target);
}

void Texture2D::setParameter(const GLenum param, const GLint value) {
    Texture::setParameter(target, param, value);
}

int Texture2D::getWidth() const {
    return width;
}

int Texture2D::getHeight() const {
    return height;
}

int Texture2D::getChannels() const {
    return channels;
}

void Texture2D::uploadTextureData(const unsigned char* data) const {
    bind();
    glTexImage2D(
        target,
        0,
        getInternalFormat(channels),
        width,
        height,
        0,
        getFormat(channels),
        GL_UNSIGNED_BYTE,
        data);
}
