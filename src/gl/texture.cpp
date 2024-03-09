#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

Texture::Texture(const string& path) {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    if (!data) {
        std::cerr << "Failed to load texture " << path << std::endl;
        return;
    }

    auto format = channels == 4 ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
}

Texture::~Texture() {
    glDeleteTextures(1, &texture);
}

void Texture::bind() const {
    glBindTexture(GL_TEXTURE_2D, texture);
}

void Texture::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
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
