#include "texture.h"

#include "log.h"
#include "stb_image.h"

Texture::Texture() {
    glGenTextures(1, &texture);
}

Texture::~Texture() {
    glDeleteTextures(1, &texture);
}

GLuint Texture::getTexture() const {
    return texture;
}

void Texture::bind(const GLenum slot, const GLenum target) const {
    glActiveTexture(slot);
    glBindTexture(target, texture);
}

void Texture::unbind(const GLenum slot, const GLenum target) {
    glActiveTexture(slot);
    glBindTexture(target, 0);
}

// Sets texture parameter (must bind to texture before use)
void Texture::setParameter(const GLenum target, const GLenum name, const GLint value) {
    glTexParameteri(target, name, value);
}

GLint Texture::getInternalFormat(int channels) {
    switch (channels) {
    case 4:
        return GL_RGBA8;
    case 3:
        return GL_RGB8;
    case 2:
        return GL_RG8;
    case 1:
        return GL_R8;
    default:
        LOG_WARN("Unexpected texture channels count {}", channels);
        return GL_RGB8;
    }
}

GLenum Texture::getFormat(int channels) {
    switch (channels) {
        case 4:
        return GL_RGBA;
        case 3:
        return GL_RGB;
        case 2:
        return GL_RG;
        case 1:
        return GL_RED;
        default:
        LOG_WARN("Unexpected texture channels count {}", channels);
        return GL_RGB;
    }
}

unsigned char* Texture::loadFile(const string& path, int& width, int& height, int& channels) {
    auto data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    if (!data) {
        LOG_ERROR("Failed to load texture {}", path);
        return nullptr;
    }
    return data;
}
