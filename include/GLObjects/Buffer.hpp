#ifndef BUFFER_H
#define BUFFER_H

#include <cstddef>
#include <glad/glad.h>



enum class BufferUsage : GLenum {
    streamDraw = GL_STREAM_DRAW,
    streamRead = GL_STREAM_READ,
    streamCopy = GL_STREAM_COPY,
    staticDraw = GL_STATIC_DRAW,
    staticRead = GL_STATIC_READ,
    staticCopy = GL_STATIC_COPY,
    dynamicDraw = GL_DYNAMIC_DRAW,
    dynamicRead = GL_DYNAMIC_READ,
    dynamicCopy = GL_DYNAMIC_COPY
};


enum class UniqueBufferUsage : GLbitfield {
    dynamicStorage = GL_DYNAMIC_STORAGE_BIT,
    mapRead = GL_MAP_READ_BIT,
    mapWrite = GL_MAP_WRITE_BIT,
    mapPersistent = GL_MAP_PERSISTENT_BIT,
    mapCoherent = GL_MAP_COHERENT_BIT,
    clientStorage = GL_CLIENT_STORAGE_BIT,
    none = 0
};


enum class BufferTarget : GLenum {
    vertices = GL_ARRAY_BUFFER,
    indices = GL_ELEMENT_ARRAY_BUFFER,
    storage = GL_SHADER_STORAGE_BUFFER,
    uniform = GL_UNIFORM_BUFFER,
    commands = GL_DRAW_INDIRECT_BUFFER,
    dispatch = GL_DISPATCH_INDIRECT_BUFFER,
    parameters = GL_PARAMETER_BUFFER,
    pixelPack = GL_PIXEL_PACK_BUFFER,
    pixelUnpack = GL_PIXEL_UNPACK_BUFFER,
    query = GL_QUERY_BUFFER,
    texture = GL_TEXTURE_BUFFER,
    transformFeedback = GL_TRANSFORM_FEEDBACK_BUFFER,
    counter = GL_ATOMIC_COUNTER_BUFFER,
    copyRead = GL_COPY_READ_BUFFER,
    copyWrite = GL_COPY_WRITE_BUFFER,
};



struct IndirectDrawArgs {
    uint32_t  count;
    uint32_t  instanceCount;
    uint32_t  first;
    uint32_t  baseInstance;
};


struct IndirectIndexedDrawArgs {
    uint32_t count;
    uint32_t instanceCount;
    uint32_t firstIndex;
    int32_t baseVertex;
    uint32_t baseInstance;
};



class Buffer {

public:
    /**
     * @brief Create a new buffer
    **/
    explicit Buffer(BufferTarget target);

    /**
     * @brief Set the data of the buffer. No more calls to setDataUnique() can then be made.
     * @param data Data to store in the buffer
     * @param size Size of the data (in bytes)
     * @param usage Usage of the buffer
    **/
    void setData(void* data, size_t size, BufferUsage usage);

    /**
     * @brief Set the data of the buffer. No more calls to setData() or setDataUnique() can then be made.
     * @param data Data to store in the buffer
     * @param size Size of the data (in bytes)
     * @param usage Usage of the buffer
    **/
    void setDataUnique(void* data, size_t size, UniqueBufferUsage usage);

    /**
     * @brief Modify a part of the buffer without resizing it
     * @param data New data to store in the buffer
     * @param offset Start of the part of the buffer to modify (in bytes)
     * @param size Size of the part of the buffer to modify (in bytes)
    **/
    void modifyData(void* data, size_t offset, size_t size);

    /**
     * @brief Use the buffer for future OpenGL calls
    **/
    void bind();

    /**
     * @brief Stop using the buffer for future OpenGL calls
    **/
    void unbind();

    /**
     * @brief Delete the buffer
    **/
    void dispose();

protected:
    GLuint buffer;
    BufferTarget target;

};



class VerticesBuffer : public Buffer {

public:
    VerticesBuffer() : Buffer(BufferTarget::vertices) {}

};



class IndicesBuffer : public Buffer {

public:
    IndicesBuffer() : Buffer(BufferTarget::indices) {}

};



class StorageBuffer : public Buffer {

public:
    /**
     * @brief Create a new storage buffer
     * @param binding Binding of the buffer in the shader(s)
    **/
    explicit StorageBuffer(int binding);

    /**
     * @brief Use the storage buffer for future OpenGL calls
    **/
    void bind();

    /**
     * @brief Stop using the storage buffer for future OpenGL calls
    **/
    void unbind();

public:
    GLuint binding;

};



class CommandsBuffer : public Buffer {

public:
    int stride;

    /**
     * @brief Create a new commands buffer
     * @param stride Size of a command (in bytes)
    **/
    explicit CommandsBuffer(int stride) : Buffer(BufferTarget::commands), stride(stride) {}

};



class ParametersBuffer : public Buffer {

public:
    int stride;

    /**
     * @brief Create a new parameters buffer
     * @param stride Size of a parameter (in bytes)
    **/
    explicit ParametersBuffer(int stride) : Buffer(BufferTarget::parameters), stride(stride) {}

};



#endif // BUFFER_H