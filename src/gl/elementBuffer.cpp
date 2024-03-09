#include "elementBuffer.h"

ElementBuffer::ElementBuffer() {
    glGenBuffers(1, &id);
}

ElementBuffer::~ElementBuffer() {
    glDeleteBuffers(1, &id);
}

GLuint ElementBuffer::getId() const {
    return id;
}

bool ElementBuffer::isInitialized() const {
    return initialized;
}

void ElementBuffer::bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void ElementBuffer::unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ElementBuffer::bufferData(GLsizeiptr size, const void* data) {
    bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    initialized = true;
}
