#include "GLObjects/Buffer.hpp"

#include <vector>
#include <cstdint>
#include <glad/glad.h>

using namespace std;



GLenum bufferTargets[] = {
    GL_SHADER_STORAGE_BUFFER,
    GL_UNIFORM_BUFFER,
    GL_ATOMIC_COUNTER_BUFFER,
    GL_TRANSFORM_FEEDBACK_BUFFER,
    GL_ARRAY_BUFFER,
    GL_ARRAY_BUFFER,
    GL_ELEMENT_ARRAY_BUFFER,
    GL_DRAW_INDIRECT_BUFFER,
    GL_DISPATCH_INDIRECT_BUFFER,
    GL_PARAMETER_BUFFER,
    GL_PIXEL_PACK_BUFFER,
    GL_PIXEL_UNPACK_BUFFER,
    GL_QUERY_BUFFER,
    GL_TEXTURE_BUFFER,
    GL_COPY_READ_BUFFER,
    GL_COPY_WRITE_BUFFER
};


GLenum indexTypes[] = {
    GL_UNSIGNED_BYTE,
    GL_UNSIGNED_SHORT,
    GL_UNSIGNED_INT
};

int indexSizes[] = {
    1,
    2,
    4
};



static GLuint boundBuffers[16] = { 
    (GLuint)-1, (GLuint)-1, (GLuint)-1, (GLuint)-1, (GLuint)-1, (GLuint)-1, (GLuint)-1, (GLuint)-1, 
    (GLuint)-1, (GLuint)-1, (GLuint)-1, (GLuint)-1, (GLuint)-1, (GLuint)-1, (GLuint)-1, (GLuint)-1 
};
static vector<GLuint> shaderBoundBuffers[4] = { vector<GLuint>() };



Buffer::Buffer(BufferTarget target, int stride) : 
    stride(stride),
    target((int)target) {
    glGenBuffers(1, &buffer);
}

Buffer::Buffer(Buffer buffer, BufferTarget target, int stride) :
    stride(stride),
    buffer(buffer.buffer),
    target((int)target) {
}


void Buffer::setData(void* data, uint32_t size, BufferUsage usage) {
    bind();
    glBufferData(bufferTargets[target], size * stride, data, (GLenum)usage);
}


void Buffer::setDataUnique(void* data, uint32_t size, UniqueBufferUsage usage) {
    bind();
    glBufferStorage(bufferTargets[target], size * stride, data, (GLbitfield)usage);
}


void Buffer::modifyData(void* data, uint32_t offset, uint32_t size) {
    bind();
    glBufferSubData(bufferTargets[target], offset * stride, size * stride, data);
}


void* Buffer::getData(uint32_t offset, uint32_t size) {
    bind();
    void* data = new char[size];
    glGetBufferSubData(bufferTargets[target], offset * stride, size * stride, data);
    return data;
}


void Buffer::clearData(uint32_t offset, uint32_t size) {
    bind();
    char zero = 0;
    glClearBufferSubData(bufferTargets[target], GL_R8, offset * stride, size * stride, GL_RED, GL_UNSIGNED_BYTE, &zero);
}


void Buffer::bind() {
    if (boundBuffers[target] != buffer) {
        glBindBuffer(bufferTargets[target], buffer);
        boundBuffers[target] = buffer;
    }
}


void Buffer::dispose() {
    glDeleteBuffers(1, &buffer);
}


// Add new elements to shaderBoundBuffers if necessary
static void addShaderBinding(int target, int binding) {
    int nextBinding = (int)shaderBoundBuffers[target].size();
    if (binding >= nextBinding) {
        for (int i = nextBinding; i <= binding; i++) {
            shaderBoundBuffers[target].push_back((GLuint)-1);
        }
    }
}

ShaderBuffer::ShaderBuffer(BufferTarget target, int binding, int stride) :
    Buffer(target, stride), 
    binding(binding) {
    addShaderBinding((int)target, binding);
}

ShaderBuffer::ShaderBuffer(Buffer buffer, BufferTarget target, int binding, int stride) :
    Buffer(buffer, target, stride), 
    binding(binding) {
    addShaderBinding((int)target, binding);
}


void ShaderBuffer::shaderBind() {
    if (shaderBoundBuffers[target][binding] != buffer) {
        glBindBufferBase(bufferTargets[target], binding, buffer);
        shaderBoundBuffers[target][binding] = buffer;
    }
}