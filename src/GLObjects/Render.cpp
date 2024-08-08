#include "GLObjects/Render.hpp"

#include <stdexcept>
#include <glad/glad.h>

#include "GLObjects/Buffer.hpp"
#include "GLObjects/VertexArray.hpp"
#include "GLObjects/Shader.hpp"

using namespace std;


void renderInit(int width, int height) {
    if (gladLoadGL() == 0) throw runtime_error("gladLoadGLLoader error");
    glViewport(0, 0, width, height);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}


void renderBackground(float r, float g, float b) {
    glClearColor(r, g, b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void renderIndirect(GeometryMode geometryMode, Shader shader, VertexArray vertexArray, CommandsBuffer commands, int startCommand, int commandCount, StorageBuffer* storageBuffers, int numStorageBuffers) {
    commands.bind();
    shader.use();
    vertexArray.bind();
    if (storageBuffers != nullptr) {
        for (int i = 0; i < numStorageBuffers; i++) {
            storageBuffers[i].bind();
        }
    }

    if (commandCount == 1) {
        if (vertexArray.indexed) glDrawElementsIndirect((GLenum)geometryMode, (GLenum)vertexArray.indexType, reinterpret_cast<void*>(startCommand * commands.stride));
        else glDrawArraysIndirect((GLenum)geometryMode, reinterpret_cast<void*>(startCommand * commands.stride));
    }
    else {
        if (vertexArray.indexed) glMultiDrawElementsIndirect((GLenum)geometryMode, (GLenum)vertexArray.indexType, reinterpret_cast<void*>(startCommand * commands.stride), commandCount, commands.stride);
        else glMultiDrawArraysIndirect((GLenum)geometryMode, reinterpret_cast<void*>(startCommand * commands.stride), commandCount, commands.stride);
    }
}


void renderIndirect(GeometryMode geometryMode, Shader shader, VertexArray vertexArray, CommandsBuffer commands, int startCommand, ParametersBuffer parameters, int parameterIndex, int maxCommandCount, StorageBuffer* storageBuffers, int numStorageBuffers) {
    shader.use();
    vertexArray.bind();
    commands.bind();
    parameters.bind();
    if (storageBuffers != nullptr) {
        for (int i = 0; i < numStorageBuffers; i++) {
            storageBuffers[i].bind();
        }
    }

    if (vertexArray.indexed) glMultiDrawElementsIndirectCount((GLenum)geometryMode, (GLenum)vertexArray.indexType, reinterpret_cast<void*>(startCommand * parameters.stride), parameterIndex * parameters.stride, maxCommandCount, commands.stride);
    else glMultiDrawArraysIndirectCount((GLenum)geometryMode, reinterpret_cast<void*>(startCommand * parameters.stride), parameterIndex * parameters.stride, maxCommandCount, commands.stride);
}