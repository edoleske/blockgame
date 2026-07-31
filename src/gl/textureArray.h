#ifndef BLOCKGAME_TEXTUREARRAY_H
#define BLOCKGAME_TEXTUREARRAY_H

#include "common.h"


class TextureArray {
public:
     TextureArray();

    ~TextureArray();

    // Copying object deletes texture
    TextureArray(const TextureArray&) = delete;
    TextureArray(TextureArray&&) = delete;
    TextureArray& operator=(const TextureArray&) = delete;

    void allocate(int resolution, int layers);

    void addLayer(const string& path);

    void bind() const;

    static void unbind();

    GLuint getTexture() const;

    int getLayerCount() const;

    int getWidth() const;

    int getHeight() const;

private:
    GLuint texture{};
    int width{}, height{}, layerCount{}, layerIndex{};

    static inline GLenum target = GL_TEXTURE_2D_ARRAY;
};


#endif //BLOCKGAME_TEXTUREARRAY_H
