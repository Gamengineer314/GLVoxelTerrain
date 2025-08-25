#include "GLObjects/Command.hpp"

#include <cstdint>
#include <stdexcept>
#include <unordered_map>
#include <glad/glad.h>

#include "GLObjects/Buffer.hpp"
#include "GLObjects/VertexArray.hpp"
#include "GLObjects/Shader.hpp"
#include "GLObjects/Log.hpp"

using namespace std;


void commandInit(int width, int height) {
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


// Currently bound objects to avoid unnecessary bind functions
static uint32_t boundArray = 0;
static uint32_t boundShader = 0;
static uint32_t boundCommandsDraw = 0;
static uint32_t boundCommandsDispatch = 0;
static uint32_t boundParameters = 0;
static unordered_map<uint32_t, const Buffer*> boundStorageBuffers;
static unordered_map<uint32_t, const Buffer*> boundUniformBuffers;
static unordered_map<uint32_t, const Buffer*> boundCountersBuffers;
static unordered_map<uint32_t, const Buffer*> boundFeedbackBuffers;


static void commandBind(const Shader& shader) {
    if (boundShader != shader.id()) {
        boundShader = shader.id();
        shader.use();
    }
    
    for (ShaderBuffer buffer : shader.buffers()) {
        if (buffer.type == ShaderBufferType::storage) {
            if (boundStorageBuffers[buffer.index] != buffer.buffer) {
                boundStorageBuffers[buffer.index] = buffer.buffer;
                buffer.buffer->use(buffer.type, buffer.index);
            }
        }
        else if (buffer.type == ShaderBufferType::uniform) {
            if (boundUniformBuffers[buffer.index] != buffer.buffer) {
                boundUniformBuffers[buffer.index] = buffer.buffer;
                buffer.buffer->use(buffer.type, buffer.index);
            }
        }
        else if (buffer.type == ShaderBufferType::counters) {
            if (boundCountersBuffers[buffer.index] != buffer.buffer) {
                boundCountersBuffers[buffer.index] = buffer.buffer;
                buffer.buffer->use(buffer.type, buffer.index);
            }
        }
        else {
            if (boundFeedbackBuffers[buffer.index] != buffer.buffer) {
                boundFeedbackBuffers[buffer.index] = buffer.buffer;
                buffer.buffer->use(buffer.type, buffer.index);
            }
        }
    }
}


void commandBind(const GraphicsShader& shader, const VertexArray& array, const Buffer* commands, const Buffer* parameters) {
    commandBind(shader);
    if (boundArray != array.id()) {
        boundArray = array.id();
        array.use();
    }
    if (commands && boundCommandsDraw != commands->id()) {
        boundCommandsDraw = commands->id();
        commands->use(BufferType::indirectDraw);
    }
    if (parameters && boundParameters != parameters->id()) {
        boundParameters = parameters->id();
        parameters->use(BufferType::parameters);
    }
}


void commandBind(const ComputeShader& shader, const Buffer* commands) {
    commandBind(shader);
    if (commands && boundCommandsDispatch != commands->id()) {
        boundCommandsDispatch = commands->id();
        commands->use(BufferType::indirectDispatch);
    }
}