#include "vertexArray.h"

VertexArray::VertexArray() {
    glGenVertexArrays(1, &id);
}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &id);
    unbind();
}

void VertexArray::bind() const {
    glBindVertexArray(id);
}

void VertexArray::unbind() {
    glBindVertexArray(0);
}
