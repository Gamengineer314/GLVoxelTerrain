#ifndef RENDER_H
#define RENDER_H

#include <cstdint>
#include <glad/glad.h>

#include "GLObjects/Buffer.hpp"
#include "GLObjects/VertexArray.hpp"
#include "GLObjects/Shader.hpp"

using namespace std;


enum class GeometryMode : GLenum {
    points = GL_POINTS,
    lineStrip = GL_LINE_STRIP, 
    lineLoop = GL_LINE_LOOP, 
    lines = GL_LINES, 
    lineStripAdjacency = GL_LINE_STRIP_ADJACENCY, 
    linesAdjacency = GL_LINES_ADJACENCY, 
    triangleStrip = GL_TRIANGLE_STRIP, 
    triangleFan = GL_TRIANGLE_FAN, 
    triangles = GL_TRIANGLES, 
    triangleStripAdjacency = GL_TRIANGLE_STRIP_ADJACENCY, 
    trianglesAdjacency = GL_TRIANGLES_ADJACENCY,
    patches = GL_PATCHES
};


enum class MemoryBarrier : GLbitfield {
    vertices = GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT,
    indexType = GL_ELEMENT_ARRAY_BARRIER_BIT,
    uniform = GL_UNIFORM_BARRIER_BIT,
    textureFetch = GL_TEXTURE_FETCH_BARRIER_BIT,
    imageAccess = GL_SHADER_IMAGE_ACCESS_BARRIER_BIT,
    indirectCommand = GL_COMMAND_BARRIER_BIT,
    pixel = GL_PIXEL_BUFFER_BARRIER_BIT,
    textureUpdate = GL_TEXTURE_UPDATE_BARRIER_BIT,
    bufferUpdate = GL_BUFFER_UPDATE_BARRIER_BIT,
    framebuffer = GL_FRAMEBUFFER_BARRIER_BIT,
    transformFeedback = GL_TRANSFORM_FEEDBACK_BARRIER_BIT,
    counter = GL_ATOMIC_COUNTER_BARRIER_BIT,
    storage = GL_SHADER_STORAGE_BARRIER_BIT,
    all = GL_ALL_BARRIER_BITS
};

constexpr MemoryBarrier operator|(MemoryBarrier barrier1, MemoryBarrier barrier2) {
    return static_cast<MemoryBarrier>((GLbitfield)barrier1 | (GLbitfield)barrier2);
}


/**
 * @brief Bind everything that needs to be bound before a draw command
**/
void commandBind(const GraphicsShader& shader, const VertexArray& array, const Buffer* commands, const Buffer* parameters);

/**
 * @brief Bind everything that needs to be bound before a compute command
**/
void commandBind(const ComputeShader& shader, const Buffer* commands);


/**
 * @brief Initialize glad and OpenGL
 * @param width Window width (in pixels)
 * @param height Window height (in pixels)
**/
void commandInit(int width, int height);


/**
 * @brief Render background color
 * @param r Red component
 * @param g Green component
 * @param b Blue component
**/
inline void commandBackground(float r, float g, float b) {
    glClearColor(r, g, b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


/**
 * @brief Draw call
 * @param mode Geometry to draw
 * @param shader Shader to use
 * @param array Vertex array to use
 * @param count Number of vertices to draw
 * @param first Index of the first vertex to draw
 * @param baseVertex For indexed rendering, index of the first vertex to draw
**/
inline void commandDraw(GeometryMode mode, const GraphicsShader& shader, const VertexArray& array, uint32_t count, uint32_t first = 0, int32_t baseVertex = 0) {
    commandBind(shader, array, NULL, NULL);
    IndexType indexType = array.indices();
    if (indexType == IndexType::none) glDrawArrays((GLenum)mode, first, count);
    else {
        int indexSize = indexType == IndexType::uint8 ? 1 : indexType == IndexType::uint16 ? 2 : 4;
        if (baseVertex == 0) glDrawElements((GLenum)mode, count, (GLenum)indexType, reinterpret_cast<void*>(first * indexSize));
        else glDrawElementsBaseVertex((GLenum)mode, count, (GLenum)indexType, reinterpret_cast<void*>(first * indexSize), baseVertex);
    }
}


/**
 * @brief Instanced draw call
 * @param mode Geometry to draw
 * @param shader Shader to use
 * @param array Vertex array to use
 * @param vertexCount Number of vertices to draw
 * @param instanceCount Number of instances to draw
 * @param firstVertex Index of the first vertex to draw
 * @param baseVertex For indexed rendering, index of the first vertex to draw
 * @param baseInstance Index of the first instance
**/
inline void commandDrawInstanced(GeometryMode mode, const GraphicsShader& shader, const VertexArray& array, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex = 0, uint32_t baseVertex = 0, uint32_t baseInstance = 0) {
    commandBind(shader, array, NULL, NULL);
    IndexType indexType = array.indices();
    if (indexType == IndexType::none) {
        if (baseInstance == 0) glDrawArraysInstanced((GLenum)mode, firstVertex, vertexCount, instanceCount);
        else glDrawArraysInstancedBaseInstance((GLenum)mode, firstVertex, vertexCount, instanceCount, baseInstance);
    }
    else {
        int indexSize = indexType == IndexType::uint8 ? 1 : indexType == IndexType::uint16 ? 2 : 4;
        if (baseInstance == 0) {
            if (baseVertex == 0) glDrawElementsInstanced((GLenum)mode, vertexCount, (GLenum)indexType, reinterpret_cast<void*>(firstVertex * indexSize), instanceCount);
            else glDrawElementsInstancedBaseVertex((GLenum)mode, vertexCount, (GLenum)indexType, reinterpret_cast<void*>(firstVertex * indexSize), instanceCount, baseVertex);
        }
        else {
            if (baseVertex == 0) glDrawElementsInstancedBaseInstance((GLenum)mode, vertexCount, (GLenum)indexType, reinterpret_cast<void*>(firstVertex * indexSize), instanceCount, baseInstance);
            else glDrawElementsInstancedBaseVertexBaseInstance((GLenum)mode, vertexCount, (GLenum)indexType, reinterpret_cast<void*>(firstVertex * indexSize), instanceCount, baseVertex, baseInstance);
        }
    }
}


/**
 * @brief Indirect draw call with commands provided in a buffer
 * @param mode Geometry to draw
 * @param shader Shader to use
 * @param array Vertex array to use
 * @param commands Indirect draw commands
 * @param commandCount Number of commands to draw
 * @param startCommand Index of the first command to draw
 * @param commandStride Size of a command if larger than the default size
**/
inline void commandDrawIndirect(GeometryMode mode, const GraphicsShader& shader, const VertexArray& array, const Buffer& commands, uint32_t commandCount = 1, uint32_t startCommand = 0, uint32_t commandStride = 0) {
    commandBind(shader, array, &commands, NULL);
    IndexType indexType = array.indices();
    if (indexType == IndexType::none) {
        if (commandCount == 1) glDrawArraysIndirect((GLenum)mode, reinterpret_cast<void*>(startCommand * commandStride));
        else glMultiDrawArraysIndirect((GLenum)mode, reinterpret_cast<void*>(startCommand * commandStride), commandCount, commandStride);
    }
    else {
        if (commandCount == 1) glDrawElementsIndirect((GLenum)mode, (GLenum)indexType, reinterpret_cast<void*>(startCommand * commandStride));
        else glMultiDrawElementsIndirect((GLenum)mode, (GLenum)indexType, reinterpret_cast<void*>(startCommand * commandStride), commandCount, commandStride);
    }
}


/**
 * @brief Indirect draw call with commands and command count provided in buffers
 * @param mode Geometry to draw
 * @param shader Shader to use
 * @param array Vertex array to use
 * @param commands Indirect draw commands
 * @param parameters Command count parameter
 * @param maxCommandCount Maximum number of commands to draw
 * @param startCommand Index of the first command to draw
 * @param parameterIndex Index of the parameter to use
 * @param commandStride Size of a command if larger than the default size
 * @param parameterStride Size of a parameter if larger than the default size
**/
inline void commandDrawIndirect(GeometryMode mode, const GraphicsShader& shader, const VertexArray& array, const Buffer& commands, const Buffer& parameters, uint32_t maxCommandCount, uint32_t startCommand = 0, uint32_t parameterIndex = 0, uint32_t commandStride = 0, uint32_t parameterStride = 0) {
    commandBind(shader, array, &commands, &parameters);
    IndexType indexType = array.indices();
    if (indexType == IndexType::none) glMultiDrawArraysIndirectCount((GLenum)mode, reinterpret_cast<void*>(startCommand * parameterStride), parameterIndex * parameterStride, maxCommandCount, commandStride);
    else glMultiDrawElementsIndirectCount((GLenum)mode, (GLenum)indexType, reinterpret_cast<void*>(startCommand * parameterStride), parameterIndex * parameterStride, maxCommandCount, commandStride);
}


/**
 * @brief Dispatch a compute shader
 * @param shader Shader to use
 * @param numGroupsX Number of work groups in the X dimension
 * @param numGroupsY Number of work groups in the Y dimension
 * @param numGroupsZ Number of work groups in the Z dimension
**/
inline void commandCompute(const ComputeShader& shader, int numGroupsX = 1, int numGroupsY = 1, int numGroupsZ = 1) {
    commandBind(shader, NULL);
    glDispatchCompute(numGroupsX, numGroupsY, numGroupsZ);
}


/**
 * @brief Dispatch a compute shader with commands provided as a buffer
 * @param shader Shader to use
 * @param commands Indirect dispatch commands
 * @param commandIndex Index of the command to use
**/
inline void commandComputeIndirect(const ComputeShader& shader, const Buffer& commands, uint32_t commandIndex = 0) {
    commandBind(shader, &commands);
    glDispatchComputeIndirect(commandIndex);
}


/**
 * @brief Use a memory barrier to order memory accesses
 * @param barrier Memory barrier to use
**/
inline void commandBarrier(MemoryBarrier barrier) {
    glMemoryBarrier((GLbitfield)barrier);
}


#endif // RENDER_H