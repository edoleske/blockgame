#ifndef BLOCKGAME_ELEMENTBUFFER_H
#define BLOCKGAME_ELEMENTBUFFER_H


#include "common.h"

class ElementBuffer {
public:
    ElementBuffer();
    ~ElementBuffer();

    GLuint getId() const;

    bool isInitialized() const;

    void bind() const;
    static void unbind();

    void bufferData(GLsizeiptr size, const void* data);
private:
    GLuint id = 0;
    bool initialized = false;
};


#endif //BLOCKGAME_ELEMENTBUFFER_H
