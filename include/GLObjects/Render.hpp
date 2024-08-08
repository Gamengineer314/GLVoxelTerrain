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
 * @brief Indirect draw call with commands provided as a buffer
 * @param geometryMode Geometry to draw
 * @param shader Shader to use
 * @param vertexArray Vertex array to use
 * @param commands Indirect draw command(s)
 * @param startCommand Index of the first command to draw
 * @param commandCount Number of commands to draw
 * @param storageBuffers Storage buffers to use
 * @param numStorageBuffers Number of elements in storageBuffers
**/
void renderIndirect(GeometryMode geometryMode, Shader shader, VertexArray vertexArray, CommandsBuffer commands, int startCommand, int commandCount, StorageBuffer* storageBuffers = nullptr, int numStorageBuffers = 0);

/**
 * @brief Indirect draw call with commands and command count provided as buffers
 * @param geometryMode Geometry to draw
 * @param shader Shader to use
 * @param vertexArray Vertex array to use
 * @param commands Indirect draw command(s)
 * @param startCommand Index of the first command to draw
 * @param parameters Command count parameter
 * @param parameterIndex Index of the parameter to use
 * @param maxCommandCount Maximum number of commands to draw
 * @param storageBuffers Storage buffers to use
 * @param numStorageBuffers Number of elements in storageBuffers
**/
void renderIndirect(GeometryMode geometryMode, Shader shader, VertexArray vertexArray, CommandsBuffer commands, ParametersBuffer parameters, int parameterIndex, int maxCommandCount, StorageBuffer* storageBuffers = nullptr, int numStorageBuffers = 0);



#endif // RENDER_H