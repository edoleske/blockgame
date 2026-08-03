#ifndef BLOCKGAME_TEXTUREARRAY_H
#define BLOCKGAME_TEXTUREARRAY_H

#include "common.h"
#include "texture.h"


class TextureArray : public Texture {
public:
     TextureArray() = default;

    explicit TextureArray(GLenum slot);

    void allocate(int resolution, int layers);

    void addLayer(const string& path);

    void bind() const;

    void unbind() const;

    // Sets texture parameter (must bind to texture before use)
    static void setParameter(GLenum param, GLint value);

    int getLayerCount() const;

    int getWidth() const;

    int getHeight() const;

private:
    GLenum slot = GL_TEXTURE0;
    int width{}, height{}, layerCount{}, layerIndex{};

    static inline GLenum target = GL_TEXTURE_2D_ARRAY;
};


#endif //BLOCKGAME_TEXTUREARRAY_H
