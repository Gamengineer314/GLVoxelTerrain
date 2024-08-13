#include "GLObjects/Render.hpp"

#include <cstdint>
#include <stdexcept>
#include <glad/glad.h>

#include "GLObjects/Buffer.hpp"
#include "GLObjects/VertexArray.hpp"
#include "GLObjects/Shader.hpp"
#include "GLObjects/Log.hpp"

using namespace std;



void renderInit(int width, int height) {
    if (gladLoadGL() == 0) throw runtime_error("gladLoadGLLoader error");
    glViewport(0, 0, width, height);
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(glCallback, 0);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}


void renderBackground(float r, float g, float b) {
    glClearColor(r, g, b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void bind(GraphicsShader shader, VertexArray vertexArray, ShaderBuffer* shaderBuffers, int numShaderBuffers) {
    shader.use();
    vertexArray.bind();
    if (shaderBuffers != nullptr) {
        for (int i = 0; i < numShaderBuffers; i++) {
            shaderBuffers[i].shaderBind();
        }
    }
}


void render(GeometryMode geometryMode, GraphicsShader shader, VertexArray vertexArray, uint32_t first, uint32_t count, uint32_t baseVertex, ShaderBuffer* shaderBuffers, int numShaderBuffers) {
    bind(shader, vertexArray, shaderBuffers, numShaderBuffers);
    if (vertexArray.indexed) glDrawElements((GLenum)geometryMode, count, indexTypes[vertexArray.indexType], reinterpret_cast<void*>(first * indexSizes[vertexArray.indexType]));
    else glDrawArrays((GLenum)geometryMode, first, count);
}


void renderInstanced(GeometryMode geometryMode, GraphicsShader shader, VertexArray vertexArray, uint32_t first, uint32_t count, uint32_t baseVertex, uint32_t baseInstance, uint32_t instanceCount, ShaderBuffer* shaderBuffers, int numShaderBuffers) {
    bind(shader, vertexArray, shaderBuffers, numShaderBuffers);
    if (baseInstance == 0) {
        if (vertexArray.indexed) {
            if (baseVertex == 0) glDrawElementsInstanced((GLenum)geometryMode, count, indexTypes[vertexArray.indexType], reinterpret_cast<void*>(first * indexSizes[vertexArray.indexType]), instanceCount);
            else glDrawElementsInstancedBaseVertex((GLenum)geometryMode, count, indexTypes[vertexArray.indexType], reinterpret_cast<void*>(first * indexSizes[vertexArray.indexType]), instanceCount, baseVertex);
        }
        else glDrawArraysInstanced((GLenum)geometryMode, first, count, instanceCount);
    }
    else {
        if (vertexArray.indexed) {
            if (baseVertex == 0) glDrawElementsInstancedBaseInstance((GLenum)geometryMode, count, indexTypes[vertexArray.indexType], reinterpret_cast<void*>(first * indexSizes[vertexArray.indexType]), instanceCount, baseInstance);
            else glDrawElementsInstancedBaseVertexBaseInstance((GLenum)geometryMode, count, indexTypes[vertexArray.indexType], reinterpret_cast<void*>(first * indexSizes[vertexArray.indexType]), instanceCount, baseVertex, baseInstance);
        }
        else glDrawArraysInstancedBaseInstance((GLenum)geometryMode, first, count, instanceCount, baseInstance);
    }
}


void renderIndirect(GeometryMode geometryMode, GraphicsShader shader, VertexArray vertexArray, IndirectDrawBuffer commands, int startCommand, int commandCount, ShaderBuffer* shaderBuffers, int numShaderBuffers) {
    bind(shader, vertexArray, shaderBuffers, numShaderBuffers);
    commands.bind();

    if (commandCount == 1) {
        if (vertexArray.indexed) glDrawElementsIndirect((GLenum)geometryMode, indexTypes[vertexArray.indexType], reinterpret_cast<void*>(startCommand * commands.stride));
        else glDrawArraysIndirect((GLenum)geometryMode, reinterpret_cast<void*>(startCommand * commands.stride));
    }
    else {
        if (vertexArray.indexed) glMultiDrawElementsIndirect((GLenum)geometryMode, indexTypes[vertexArray.indexType], reinterpret_cast<void*>(startCommand * commands.stride), commandCount, commands.stride);
        else glMultiDrawArraysIndirect((GLenum)geometryMode, reinterpret_cast<void*>(startCommand * commands.stride), commandCount, commands.stride);
    }
}


void renderIndirect(GeometryMode geometryMode, GraphicsShader shader, VertexArray vertexArray, IndirectDrawBuffer commands, int startCommand, ParametersBuffer parameters, int parameterIndex, int maxCommandCount, ShaderBuffer* shaderBuffers, int numShaderBuffers) {
    bind(shader, vertexArray, shaderBuffers, numShaderBuffers);
    commands.bind();
    parameters.bind();

    if (vertexArray.indexed) glMultiDrawElementsIndirectCount((GLenum)geometryMode, indexTypes[vertexArray.indexType], reinterpret_cast<void*>(startCommand * parameters.stride), parameterIndex * parameters.stride, maxCommandCount, commands.stride);
    else glMultiDrawArraysIndirectCount((GLenum)geometryMode, reinterpret_cast<void*>(startCommand * parameters.stride), parameterIndex * parameters.stride, maxCommandCount, commands.stride);
}


void memoryBarrier(MemoryBarrier barrier) {
    glMemoryBarrier((GLbitfield)barrier);
}