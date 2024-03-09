#ifndef BLOCKGAME_TEXTURE_H
#define BLOCKGAME_TEXTURE_H


#include "common.h"

class Texture {
public:
    Texture(const string& path);
    ~Texture();

    void bind() const;
    static void unbind();

    GLuint getTexture() const;

    int getWidth() const;

    int getHeight() const;

private:
    GLuint texture;
    int width, height, channels;

};


#endif //BLOCKGAME_TEXTURE_H
