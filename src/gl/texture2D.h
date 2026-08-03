#ifndef BLOCKGAME_TEXTURE2D_H
#define BLOCKGAME_TEXTURE2D_H

#include "common.h"
#include "texture.h"


class Texture2D : public Texture {
public:
    Texture2D() = default;

    explicit Texture2D(GLenum slot);

    // Loads texture image with stbi_image
    void load(const string& path);

    // Loads texture image with direct buffer data
    // WARNING: No bounds checking
    void load(const unsigned char* data, GLsizei width, GLsizei height, int channels);

    void bind() const;

    void unbind() const;

    // Sets texture parameter (must bind to texture before use)
    static void setParameter(GLenum param, GLint value);

    int getWidth() const;

    int getHeight() const;

    int getChannels() const;

private:
    GLenum slot = GL_TEXTURE0;
    int width{}, height{}, channels{};

    void uploadTextureData(const unsigned char* data) const;

    static constexpr GLenum target = GL_TEXTURE_2D;
};


#endif //BLOCKGAME_TEXTURE2D_H
