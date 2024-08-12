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


enum class BufferTarget : int { // GLenum
    storage = 0, // GL_SHADER_STORAGE_BUFFER
    uniform = 1, // GL_UNIFORM_BUFFER
    counters = 2, // GL_ATOMIC_COUNTER_BUFFER
    transformFeedback = 3, // GL_TRANSFORM_FEEDBACK_BUFFER
    vertices = 4, // GL_ARRAY_BUFFER
    instances = 5, // GL_ARRAY_BUFFER
    indices = 6, // GL_ELEMENT_ARRAY_BUFFER
    indirectDraw = 7, // GL_DRAW_INDIRECT_BUFFER
    indirectDispatch = 8, // GL_DISPATCH_INDIRECT_BUFFER
    parameters = 9, // GL_PARAMETER_BUFFER
    pixelPack = 10, // GL_PIXEL_PACK_BUFFER
    pixelUnpack = 11, // GL_PIXEL_UNPACK_BUFFER
    query = 12, // GL_QUERY_BUFFER
    texture = 13, // GL_TEXTURE_BUFFER
    copyRead = 14, // GL_COPY_READ_BUFFER
    copyWrite = 15, // GL_COPY_WRITE_BUFFER
};



struct IndirectDrawArgs {
    uint32_t count;
    uint32_t instanceCount;
    uint32_t first;
    uint32_t baseInstance;
};


struct IndirectIndexedDrawArgs {
    uint32_t count;
    uint32_t instanceCount;
    uint32_t firstIndex;
    int32_t baseVertex;
    uint32_t baseInstance;
};


struct IndirectDispatchArgs {
    uint32_t numGroupsX;
    uint32_t numGroupsY;
    uint32_t numGroupsZ;
};



class Buffer {

public:
    /**
     * @brief Create a new buffer
     * @param target Buffer target
    **/
    explicit Buffer(BufferTarget target);

    /**
     * @brief Create a new buffer referencing an existing buffer
     * @param target Target of the new buffer
     * @param buffer Buffer to reference
    **/
    Buffer(Buffer buffer, BufferTarget target);

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
     * @brief Get a part of the buffer
     * @param offset Start of the part of the buffer to get (in bytes)
     * @param size Size of the part of the buffer to get (in bytes)
     * @return Pointer to the data (must be deallocated after use)
    **/
    void* getData(size_t offset, size_t size);

    /**
     * @brief Set a part of the buffer to zero
     * @param offset Start of the part of the buffer to clear (in bytes)
     * @param size Size of the part of the buffer to clear (in bytes)
     */
    void clearData(size_t offset, size_t size);

    /**
     * @brief Use the buffer for future OpenGL calls
    **/
    void bind();

    /**
     * @brief Delete the buffer
    **/
    void dispose();

protected:
    GLuint buffer;
    int target;

};



class ShaderBuffer : public Buffer {

public:
    /**
     * @brief Create a new shader buffer
     * @param binding Binding of the buffer in the shader(s)
     * @param target Buffer target
    **/
    explicit ShaderBuffer(int binding, BufferTarget target);

    /**
     * @brief Create a new shader buffer referencing an existing buffer
     * @param binding Binding of the buffer in the shader(s)
     * @param buffer Buffer to reference
     * @param target Target of the new buffer
    **/
    ShaderBuffer(int binding, Buffer buffer, BufferTarget target);

    /**
     * @brief Use the shader buffer for future OpenGL calls
    **/
    void shaderBind();

private:
    GLuint binding;

};



class VerticesBuffer : public Buffer {

public:
    /**
     * @brief Create a new vertices buffer
    **/
    VerticesBuffer() : Buffer(BufferTarget::vertices) {}

    /**
     * @brief Create a new vertices buffer referencing an existing buffer
     * @param buffer Buffer to reference
    **/
    explicit VerticesBuffer(Buffer buffer) : Buffer(buffer, BufferTarget::vertices) {}

};



class InstancesBuffer : public Buffer {

public:
    /**
     * @brief Create a new instances buffer
    **/
    InstancesBuffer() : Buffer(BufferTarget::instances) {}

    /**
     * @brief Create a new instances buffer referencing an existing buffer
     * @param buffer Buffer to reference
    **/
    explicit InstancesBuffer(Buffer buffer) : Buffer(buffer, BufferTarget::vertices) {}

};



class IndicesBuffer : public Buffer {

public:
    /**
     * @brief Create a new indices buffer
    **/
    IndicesBuffer() : Buffer(BufferTarget::instances) {}

    /**
     * @brief Create a new indices buffer referencing an existing buffer
     * @param buffer Buffer to reference
    **/
    explicit IndicesBuffer(Buffer buffer) : Buffer(buffer, BufferTarget::vertices) {}

};



class StorageBuffer : public ShaderBuffer {

public:
    /**
     * @brief Create a new storage buffer
     * @param binding Binding of the buffer in the shader(s)
    **/
    StorageBuffer(int binding) : ShaderBuffer(binding, BufferTarget::storage) {}

    /**
     * @brief Create a new storage buffer referencing an existing buffer
     * @param binding Binding of the buffer in the shader(s)
     * @param buffer Buffer to reference
    **/
    explicit StorageBuffer(int binding, Buffer buffer) : ShaderBuffer(binding, buffer, BufferTarget::storage) {}

};



class CountersBuffer : public ShaderBuffer {

public:
    /**
     * @brief Create a new counters buffer
     * @param binding Binding of the buffer in the shader(s)
    **/
    CountersBuffer(int binding) : ShaderBuffer(binding, BufferTarget::counters) {}

    /**
     * @brief Create a new counters buffer referencing an existing buffer
     * @param binding Binding of the buffer in the shader(s)
     * @param buffer Buffer to reference
    **/
    explicit CountersBuffer(int binding, Buffer buffer) : ShaderBuffer(binding, buffer, BufferTarget::counters) {}

};



class IndirectDrawBuffer : public Buffer {

public:
    int stride;

    /**
     * @brief Create a new indirect draw buffer
     * @param stride Size of a draw command (in bytes)
    **/
    explicit IndirectDrawBuffer(int stride) : Buffer(BufferTarget::indirectDraw), stride(stride) {}

    /**
     * @brief Create a new indirect draw buffer referencing an existing buffer
     * @param stride Size of a draw command (in bytes)
     * @param buffer Buffer to reference
    **/
    IndirectDrawBuffer(int stride, Buffer buffer) : Buffer(buffer, BufferTarget::indirectDraw), stride(stride) {}

};



class IndirectDispatchBuffer : public Buffer {

public:
    int stride;

    /**
     * @brief Create a new indirect dispatch buffer
     * @param stride Size of a dispatch command (in bytes)
    **/
    explicit IndirectDispatchBuffer(int stride) : Buffer(BufferTarget::indirectDispatch), stride(stride) {}

    /**
     * @brief Create a new indirect dispatch buffer referencing an existing buffer
     * @param stride Size of a dispatch command (in bytes)
     * @param buffer Buffer to reference
    **/
    IndirectDispatchBuffer(int stride, Buffer buffer) : Buffer(buffer, BufferTarget::indirectDispatch), stride(stride) {}

};



class ParametersBuffer : public Buffer {

public:
    int stride;

    /**
     * @brief Create a new parameters buffer
     * @param stride Size of a parameter (in bytes)
    **/
    explicit ParametersBuffer(int stride) : Buffer(BufferTarget::parameters), stride(stride) {}

    /**
     * @brief Create a new parameters buffer referencing an existing buffer
     * @param stride Size of a parameter (in bytes)
     * @param buffer Buffer to reference
    **/
    ParametersBuffer(int stride, Buffer buffer) : Buffer(buffer, BufferTarget::parameters), stride(stride) {}

};



#endif // BUFFER_H