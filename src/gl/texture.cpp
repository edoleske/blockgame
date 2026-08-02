#include "texture.h"

#include "log.h"
#include "stb_image.h"

Texture::Texture(const string& path) {
    glGenTextures(1, &texture);
    bind();

    glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    auto data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    if (!data) {
        LOG_ERROR("Failed to load texture {}", path);
        return;
    }

    auto format = channels == 4 ? GL_RGBA : GL_RGB;
    glTexImage2D(target, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(target);

    stbi_image_free(data);
}

Texture::Texture(const vector<unsigned char>& atlasData, const GLsizei width, const GLsizei height) {
    glGenTextures(1, &texture);
    glBindTexture(target, texture);

    glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(target, 0, GL_R8, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, atlasData.data());

    channels = 1;
    this->width = width;
    this->height = height;
}

Texture::~Texture() {
    glDeleteTextures(1, &texture);
}

void Texture::bind() const {
    glBindTexture(target, texture);
}

void Texture::unbind() {
    glBindTexture(target, 0);
}

GLuint Texture::getTexture() const {
    return texture;
}

int Texture::getWidth() const {
    return width;
}

int Texture::getHeight() const {
    return height;
}
