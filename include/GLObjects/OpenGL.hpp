#ifndef RENDER_H
#define RENDER_H

#include <cstdint>
#include <glad/glad.h>

#include "GLObjects/Buffer.hpp"
#include "GLObjects/VertexArray.hpp"
#include "GLObjects/Shader.hpp"

namespace gl {

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
 * @brief Initialize glad and OpenGL
 * @param width Window width (in pixels)
 * @param height Window height (in pixels)
**/
void init(int width, int height);


/**
 * @brief Render background color
 * @param r Red component
 * @param g Green component
 * @param b Blue component
**/
inline void setBackground(float r, float g, float b) {
    glClearColor(r, g, b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


/**
 * @brief Draw call
 * @param mode Geometry to draw
 * @param vertexCount Number of vertices to draw
 * @param firstVertex Index of the first vertex to draw
**/
inline void draw(GeometryMode mode, uint32_t vertexCount, uint32_t firstVertex = 0) {
    glDrawArrays((GLenum)mode, firstVertex, vertexCount);
}


/**
 * @brief Indexed draw call
 * @param mode Geometry to draw
 * @param indexType Type of the indices
 * @param indexCount Number of indices to draw
 * @param firstIndex Index of the first index to draw
 * @param baseVertex Vertex index offset
**/
inline void drawIndexed(GeometryMode mode, IndexType indexType, uint32_t indexCount, uint32_t firstIndex = 0, int32_t baseVertex = 0) {
    int indexSize = indexType == IndexType::uint8 ? 1 : indexType == IndexType::uint16 ? 2 : 4;
    if (baseVertex == 0) glDrawElements((GLenum)mode, indexCount, (GLenum)indexType, reinterpret_cast<void*>(firstIndex * indexSize));
    else glDrawElementsBaseVertex((GLenum)mode, indexCount, (GLenum)indexType, reinterpret_cast<void*>(firstIndex * indexSize), baseVertex);
}


/**
 * @brief Instanced draw call
 * @param mode Geometry to draw
 * @param vertexCount Number of vertices to draw
 * @param instanceCount Number of instances to draw
 * @param firstVertex Index of the first vertex to draw
 * @param firstInstance Index of the first instance
**/
inline void drawInstanced(GeometryMode mode, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex = 0, uint32_t firstInstance = 0) {
    if (firstInstance == 0) glDrawArraysInstanced((GLenum)mode, firstVertex, vertexCount, instanceCount);
    else glDrawArraysInstancedBaseInstance((GLenum)mode, firstVertex, vertexCount, instanceCount, firstInstance);
}


/**
 * @brief Indexed instanced draw call
 * @param mode Geometry to draw
 * @param indexType Type of the indices
 * @param indexCount Number of indices to draw
 * @param instanceCount Number of instances to draw
 * @param firstIndex Index of the first index to draw
 * @param baseVertex Vertex offset
 * @param firstInstance Index of the first instance to draw
**/
inline void drawIndexedInstanced(GeometryMode mode, IndexType indexType, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex = 0, uint32_t baseVertex = 0, uint32_t firstInstance = 0) {
    int indexSize = indexType == IndexType::uint8 ? 1 : indexType == IndexType::uint16 ? 2 : 4;
    if (firstInstance == 0) {
        if (baseVertex == 0) glDrawElementsInstanced((GLenum)mode, indexCount, (GLenum)indexType, reinterpret_cast<void*>(firstIndex * indexSize), instanceCount);
        else glDrawElementsInstancedBaseVertex((GLenum)mode, indexCount, (GLenum)indexType, reinterpret_cast<void*>(firstIndex * indexSize), instanceCount, baseVertex);
    }
    else {
        if (baseVertex == 0) glDrawElementsInstancedBaseInstance((GLenum)mode, indexCount, (GLenum)indexType, reinterpret_cast<void*>(firstIndex * indexSize), instanceCount, firstInstance);
        else glDrawElementsInstancedBaseVertexBaseInstance((GLenum)mode, indexCount, (GLenum)indexType, reinterpret_cast<void*>(firstIndex * indexSize), instanceCount, baseVertex, firstInstance);
    }
}


/**
 * @brief Indirect draw call with commands provided in a buffer
 * @param mode Geometry to draw
 * @param commandCount Number of commands to draw
 * @param firstCommand Index of the first command to draw
 * @param commandStride Size of a command if larger than the default size
**/
inline void drawIndirect(GeometryMode mode, uint32_t commandCount = 1, uint32_t firstCommand = 0, uint32_t commandStride = 0) {
    if (commandCount == 1) glDrawArraysIndirect((GLenum)mode, reinterpret_cast<void*>(firstCommand * commandStride));
    else glMultiDrawArraysIndirect((GLenum)mode, reinterpret_cast<void*>(firstCommand * commandStride), commandCount, commandStride);
}


/**
 * @brief Indexed indirect draw call with commands provided in a buffer
 * @param mode Geometry to draw
 * @param indexType Type of the indices
 * @param commandCount Number of commands to draw
 * @param firstCommand Index of the first command to draw
 * @param commandStride Size of a command if larger than the default size
**/
inline void drawIndexedIndirect(GeometryMode mode, IndexType indexType, uint32_t commandCount = 1, uint32_t firstCommand = 0, uint32_t commandStride = 0) {
    if (commandCount == 1) glDrawElementsIndirect((GLenum)mode, (GLenum)indexType, reinterpret_cast<void*>(firstCommand * commandStride));
    else glMultiDrawElementsIndirect((GLenum)mode, (GLenum)indexType, reinterpret_cast<void*>(firstCommand * commandStride), commandCount, commandStride);
}


/**
 * @brief Indirect draw call with commands and command count provided in buffers
 * @param mode Geometry to draw
 * @param maxCommandCount Maximum number of commands to draw
 * @param firstCommand Index of the first command to draw
 * @param parameterIndex Index of the parameter to use
 * @param commandStride Size of a command if larger than the default size
 * @param parameterStride Size of a parameter if larger than the default size
**/
inline void drawIndirectParam(GeometryMode mode, uint32_t maxCommandCount, uint32_t firstCommand = 0, uint32_t parameterIndex = 0, uint32_t commandStride = 0, uint32_t parameterStride = 0) {
    glMultiDrawArraysIndirectCount((GLenum)mode, reinterpret_cast<void*>(firstCommand * commandStride), parameterIndex * parameterStride, maxCommandCount, commandStride);
}


/**
 * @brief Indirect draw call with commands and command count provided in buffers
 * @param mode Geometry to draw
 * @param indexType Type of the indices
 * @param maxCommandCount Maximum number of commands to draw
 * @param firstCommand Index of the first command to draw
 * @param parameterIndex Index of the parameter to use
 * @param commandStride Size of a command if larger than the default size
 * @param parameterStride Size of a parameter if larger than the default size
**/
inline void drawIndexedIndirectParam(GeometryMode mode, IndexType indexType, uint32_t maxCommandCount, uint32_t firstCommand = 0, uint32_t parameterIndex = 0, uint32_t commandStride = 0, uint32_t parameterStride = 0) {
    glMultiDrawElementsIndirectCount((GLenum)mode, (GLenum)indexType, reinterpret_cast<void*>(firstCommand * parameterStride), parameterIndex * parameterStride, maxCommandCount, commandStride);
}


/**
 * @brief Dispatch a compute shader
 * @param numGroupsX Number of work groups in the X dimension
 * @param numGroupsY Number of work groups in the Y dimension
 * @param numGroupsZ Number of work groups in the Z dimension
**/
inline void compute(uint32_t numGroupsX = 1, uint32_t numGroupsY = 1, uint32_t numGroupsZ = 1) {
    glDispatchCompute(numGroupsX, numGroupsY, numGroupsZ);
}


/**
 * @brief Dispatch a compute shader with commands provided as a buffer
 * @param commandIndex Index of the command to use
**/
inline void computeIndirect(uint32_t commandIndex = 0) {
    glDispatchComputeIndirect(commandIndex);
}


/**
 * @brief Use a memory barrier to order memory accesses
 * @param barrier Memory barrier to use
**/
inline void barrier(MemoryBarrier barrier) {
    glMemoryBarrier((GLbitfield)barrier);
}

}

#endif