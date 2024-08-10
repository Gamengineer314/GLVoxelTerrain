#include "GLObjects/VertexArray.hpp"

#include <glad/glad.h>

#include "GLObjects/Buffer.hpp"

using namespace std;

static GLuint boundArray = 0;


VertexArray::VertexArray() {
    glGenVertexArrays(1, &array);
}


void VertexArray::setVerticesAttribute(int index, VerticesBuffer buffer, AttributeType type, int size, int stride, int offset, bool normalize) {
    bind();
    buffer.bind();
    glVertexAttribPointer(index, size, (GLenum)type, normalize ? GL_TRUE : GL_FALSE, stride, reinterpret_cast<void*>(offset));
    glEnableVertexAttribArray(index);
}


void VertexArray::removeVerticesAttribute(int index) {
    bind();
    glDisableVertexAttribArray(index);
}


void VertexArray::setIndices(IndicesBuffer buffer, IndexType indexType) {
    this->indexType = indexType;
    indexed = true;
    bind();
    buffer.bind();
}


void VertexArray::removeIndices() {
    indexed = false;
    bind();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


void VertexArray::bind() {
    if (boundArray != array) {
        glBindVertexArray(array);
        boundArray = array;
    }
}


void VertexArray::unbind() {
    if (boundArray != 0) {
        glBindVertexArray(0);
        boundArray = 0;
    }
}


void VertexArray::dispose() {
    glDeleteVertexArrays(1, &array);
}