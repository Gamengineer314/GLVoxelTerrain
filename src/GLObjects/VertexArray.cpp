#include "GLObjects/VertexArray.hpp"

#include <glad/glad.h>

#include "GLObjects/Buffer.hpp"

using namespace std;

static GLuint boundArray = 0;


VertexArray::VertexArray() {
    glGenVertexArrays(1, &array);
}


void VertexArray::setVerticesFloat(int index, VerticesBuffer buffer, FloatAttributeType type, int size, int stride, int offset) {
    bind();
    buffer.bind();
    glVertexAttribPointer(index, size, (GLenum)type, GL_FALSE, stride, reinterpret_cast<void*>(offset));
    glEnableVertexAttribArray(index);
}

void VertexArray::setVerticesDouble(int index, VerticesBuffer buffer, int size, int stride, int offset) {
    bind();
    buffer.bind();
    glVertexAttribLPointer(index, size, GL_DOUBLE, stride, reinterpret_cast<void*>(offset));
    glEnableVertexAttribArray(index);
}

void VertexArray::setVerticesInt(int index, VerticesBuffer buffer, IntAttributeType type, int size, int stride, int offset) {
    bind();
    buffer.bind();
    glVertexAttribIPointer(index, size, (GLenum)type, stride, reinterpret_cast<void*>(offset));
    glEnableVertexAttribArray(index);
}

void VertexArray::setVerticesToFloat(int index, VerticesBuffer buffer, ToFloatAttributeType type, int size, int stride, int offset, bool normalize) {
    bind();
    buffer.bind();
    glVertexAttribPointer(index, size, (GLenum)type, normalize ? GL_TRUE : GL_FALSE, stride, reinterpret_cast<void*>(offset));
    glEnableVertexAttribArray(index);
}

void VertexArray::setInstancesFloat(int index, InstancesBuffer buffer, FloatAttributeType type, int size, int stride, int offset, int divisor) {
    bind();
    buffer.bind();
    glVertexAttribPointer(index, size, (GLenum)type, GL_FALSE, stride, reinterpret_cast<void*>(offset));
    glVertexAttribDivisor(index, divisor);
    glEnableVertexAttribArray(index);
}

void VertexArray::setInstancesDouble(int index, InstancesBuffer buffer, int size, int stride, int offset, int divisor) {
    bind();
    buffer.bind();
    glVertexAttribLPointer(index, size, GL_DOUBLE, stride, reinterpret_cast<void*>(offset));
    glVertexAttribDivisor(index, divisor);
    glEnableVertexAttribArray(index);
}

void VertexArray::setInstancesInt(int index, InstancesBuffer buffer, IntAttributeType type, int size, int stride, int offset, int divisor) {
    bind();
    buffer.bind();
    glVertexAttribIPointer(index, size, (GLenum)type, stride, reinterpret_cast<void*>(offset));
    glVertexAttribDivisor(index, divisor);
    glEnableVertexAttribArray(index);
}

void VertexArray::setInstancesToFloat(int index, InstancesBuffer buffer, ToFloatAttributeType type, int size, int stride, int offset, bool normalize, int divisor) {
    bind();
    buffer.bind();
    glVertexAttribPointer(index, size, (GLenum)type, normalize ? GL_TRUE : GL_FALSE, stride, reinterpret_cast<void*>(offset));
    glVertexAttribDivisor(index, divisor);
    glEnableVertexAttribArray(index);
}



void VertexArray::removeVerticesAttribute(int index) {
    bind();
    glDisableVertexAttribArray(index);
}

void VertexArray::removeInstancesAttribute(int index) {
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


void VertexArray::dispose() {
    glDeleteVertexArrays(1, &array);
}