#include "vertexBuffer.h"

VertexBuffer::VertexBuffer() {
    glGenBuffers(1, &id);
}

VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &id);
    unbind();
}

void VertexBuffer::bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, id);
}

void VertexBuffer::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::bufferData(const GLsizeiptr size, const void* data, const GLenum usage) const {
    bind();
    glBufferData(GL_ARRAY_BUFFER, size, data, usage);
}

void VertexBuffer::vertexAttribPointer(
    const GLuint index, const GLint size, const GLenum type, const GLboolean normalized, const GLsizei stride,
    const GLvoid* pointer) const {
    bind();
    glVertexAttribPointer(index, size, type, normalized, stride, pointer);
    glEnableVertexAttribArray(index);
}

void VertexBuffer::vertexAttribIPointer(
    const GLuint index, const GLint size, const GLenum type, const GLsizei stride, const GLvoid* pointer) const {
    bind();
    glVertexAttribIPointer(index, size, type, stride, pointer);
    glEnableVertexAttribArray(index);
}
