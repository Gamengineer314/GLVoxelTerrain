#include "GLObjects/Buffer.hpp"

#include <vector>
#include <cstddef>
#include <glad/glad.h>

using namespace std;

static vector<GLuint> boundStorageBuffers = vector<GLuint>();



Buffer::Buffer(BufferTarget target) : 
    target(target) {
    glGenBuffers(1, &buffer);
}


void Buffer::setData(void* data, size_t size, BufferUsage usage) {
    bind();
    glBufferData((GLenum)target, size, data, (GLenum)usage);
}


void Buffer::setDataUnique(void* data, size_t size, UniqueBufferUsage usage) {
    bind();
    glBufferStorage((GLenum)target, size, data, (GLbitfield)usage);
}


void Buffer::modifyData(void* data, size_t offset, size_t size) {
    bind();
    glBufferSubData((GLenum)target, offset, size, data);
}


void Buffer::bind() {
    glBindBuffer((GLenum)target, buffer);
}


void Buffer::unbind() {
    glBindBuffer((GLenum)target, 0);
}


void Buffer::dispose() {
    glDeleteBuffers(1, &buffer);
}



StorageBuffer::StorageBuffer(int binding) :
    Buffer(BufferTarget::storage), binding(binding) {
    if (binding > (int)boundStorageBuffers.size()) {
        for (int i = boundStorageBuffers.size(); i < binding; i++) {
            boundStorageBuffers.push_back(0);
        }
    }
}


void StorageBuffer::bind() {
    if (boundStorageBuffers[binding] != buffer) {
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, buffer);
        boundStorageBuffers[binding] = buffer;
    }
}


void StorageBuffer::unbind() {
    if (boundStorageBuffers[binding] != 0) {
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, 0);
        boundStorageBuffers[binding] = 0;
    }
}