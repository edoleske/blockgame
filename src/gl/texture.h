#ifndef BLOCKGAME_TEXTURE_H
#define BLOCKGAME_TEXTURE_H


#include "common.h"

class Texture {
public:
    Texture();

    // Creates font bitmap texture from packed font atlas (from stb_truetype)
    explicit Texture(const vector<unsigned char>& atlasData, GLsizei width, GLsizei height);

    virtual ~Texture();

    // Disallow copy/move
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;
    Texture(const Texture&&) = delete;
    Texture& operator=(const Texture&&) = delete;

    GLuint getTexture() const;

protected:
    GLuint texture{};

    void bind(GLenum slot, GLenum target) const;

    static void unbind(GLenum slot, GLenum target);

    static void setParameter(GLenum target, GLenum name, GLint value);

    static GLint getInternalFormat(int channels);

    static GLenum getFormat(int channels);

    static unsigned char* loadFile(const string& path, int& width, int& height, int& channels);
};


#endif //BLOCKGAME_TEXTURE_H
