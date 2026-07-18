#ifndef BLOCKGAME_TEXTURE_H
#define BLOCKGAME_TEXTURE_H


#include "common.h"

class Texture {
public:
    explicit Texture(const string& path);

    // Creates font bitmap texture from packed font atlas (from stb_truetype)
    explicit Texture(const vector<unsigned char>& atlasData, GLsizei width, GLsizei height);

    ~Texture();

    void bind() const;

    static void unbind();

    GLuint getTexture() const;

    int getWidth() const;

    int getHeight() const;

private:
    GLuint texture{};
    int width{}, height{}, channels{};

};


#endif //BLOCKGAME_TEXTURE_H
