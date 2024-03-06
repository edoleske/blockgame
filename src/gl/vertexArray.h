#ifndef BLOCKGAME_VERTEXARRAY_H
#define BLOCKGAME_VERTEXARRAY_H


#include "../common.h"

class VertexArray {
public:
    VertexArray();
    ~VertexArray();

    void bind() const;
    static void unbind();
private:
    GLuint id = 0;
};


#endif //BLOCKGAME_VERTEXARRAY_H
