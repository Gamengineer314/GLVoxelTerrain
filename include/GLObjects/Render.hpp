#ifndef RENDER_H
#define RENDER_H

#include <cstdint>
#include <glad/glad.h>

#include "GLObjects/Buffer.hpp"
#include "GLObjects/VertexArray.hpp"
#include "GLObjects/Shader.hpp"



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
    instances = GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT,
    indices = GL_ELEMENT_ARRAY_BARRIER_BIT,
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
void renderInit(int width, int height);


/**
 * @brief Render background color
 * @param r Red component
 * @param g Green component
 * @param b Blue component
**/
void renderBackground(float r, float g, float b);


/**
 * @brief Draw call
 * @param mode Geometry to draw
 * @param shader Shader to use
 * @param VertexArray Vertex array to use
 * @param first Index of the first vertex or index to draw
 * @param count Number of vertices to draw
 * @param baseVertex Index of the first vertex to draw for indexed rendering (ignored otherwise)
 * @param shaderBuffers Shader buffers to use
 * @param numShaderBuffers Number of elements in shaderBuffers
**/
void render(GeometryMode geometryMode, GraphicsShader shader, VertexArray vertexArray, uint32_t first, uint32_t count, uint32_t baseVertex, ShaderBuffer* shaderBuffers = nullptr, int numShaderBuffers = 0);


/**
 * @brief Instanced draw call
 * @param geometryMode Geometry to draw
 * @param shader Shader to use
 * @param vertexArray Vertex array to use
 * @param first Index of the first vertex or index to draw
 * @param count Number of vertices to draw
 * @param baseVertex Index of the first vertex to draw for indexed rendering (ignored otherwise)
 * @param baseInstance Index of the first instance
 * @param instanceCount Number of instances to draw
 * @param shaderBuffers Shader buffers to use
 * @param numShaderBuffers Number of elements in shaderBuffers
**/
void renderInstanced(GeometryMode geometryMode, GraphicsShader shader, VertexArray vertexArray, uint32_t first, uint32_t count, uint32_t baseVertex, uint32_t baseInstance, uint32_t instanceCount, ShaderBuffer* shaderBuffers = nullptr, int numShaderBuffers = 0);


/**
 * @brief Indirect draw call with commands provided in a buffer
 * @param geometryMode Geometry to draw
 * @param shader Shader to use
 * @param vertexArray Vertex array to use
 * @param commands Indirect draw command(s)
 * @param startCommand Index of the first command to draw
 * @param commandCount Number of commands to draw
 * @param shaderBuffers Shader buffers to use
 * @param numShaderBuffers Number of elements in shaderBuffers
**/
void renderIndirect(GeometryMode geometryMode, GraphicsShader shader, VertexArray vertexArray, IndirectDrawBuffer commands, int startCommand, int commandCount, ShaderBuffer* shaderBuffers = nullptr, int numShaderBuffers = 0);

/**
 * @brief Indirect draw call with commands and command count provided in buffers
 * @param geometryMode Geometry to draw
 * @param shader Shader to use
 * @param vertexArray Vertex array to use
 * @param commands Indirect draw command(s)
 * @param startCommand Index of the first command to draw
 * @param parameters Command count parameter
 * @param parameterIndex Index of the parameter to use
 * @param maxCommandCount Maximum number of commands to draw
 * @param shaderBuffers Shader buffers to use
 * @param numShaderBuffers Number of elements in shaderBuffers
**/
void renderIndirect(GeometryMode geometryMode, GraphicsShader shader, VertexArray vertexArray, IndirectDrawBuffer commands, int startCommand, ParametersBuffer parameters, int parameterIndex, int maxCommandCount, ShaderBuffer* shaderBuffers = nullptr, int numShaderBuffers = 0);


/**
 * @brief Use a memory barrier to order memory accesses
 * @param barrier Memory barrier to use
**/
void memoryBarrier(MemoryBarrier barrier);



#endif // RENDER_H