#ifndef BLOCKGAME_VERTEXBUFFER_H
#define BLOCKGAME_VERTEXBUFFER_H


#include "common.h"

class VertexBuffer {
public:
    VertexBuffer();
    ~VertexBuffer();

    void bind() const;
    static void unbind();

    void bufferData(GLsizeiptr size, void* data, GLenum usage);
    void vertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride,
                             const GLvoid* pointer);
    void vertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid* pointer);
private:
    GLuint id = 0;
};


#endif //BLOCKGAME_VERTEXBUFFER_H
