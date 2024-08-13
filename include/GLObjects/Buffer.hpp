#ifndef BUFFER_H
#define BUFFER_H

#include <cstdint>
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

extern GLenum bufferTargets[16];


enum class IndexType : GLenum {
    uint8 = 0, // GL_UNSIGNED_BYTE
    uint16 = 1, // GL_UNSIGNED_SHORT
    uint32 = 2, // GL_UNSIGNED_INT
};

extern GLenum indexTypes[3];

extern int indexSizes[3];



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
    int stride;

    /**
     * @brief Create a new buffer
     * @param target Buffer target
     * @param stride Size of an element in the buffer (in bytes)
    **/
    Buffer(BufferTarget target, int stride);

    /**
     * @brief Create a new buffer referencing an existing buffer
     * @param buffer Buffer to reference
     * @param target Target of the new buffer
     * @param stride Size of an element in the buffer (in bytes)
    **/
    Buffer(Buffer buffer, BufferTarget target, int stride);

    /**
     * @brief Set the data of the buffer. No more calls to setDataUnique() can then be made.
     * @param data Data to store in the buffer
     * @param size Number of elements to store
     * @param usage Usage of the buffer
    **/
    void setData(void* data, uint32_t size, BufferUsage usage);

    /**
     * @brief Set the data of the buffer. No more calls to setData() or setDataUnique() can then be made.
     * @param data Data to store in the buffer
     * @param size Number of elements to store
     * @param usage Usage of the buffer
    **/
    void setDataUnique(void* data, uint32_t size, UniqueBufferUsage usage);

    /**
     * @brief Modify a part of the buffer without resizing it
     * @param data New data to store in the buffer
     * @param offset First element to modify
     * @param size Number of elements to modify
    **/
    void modifyData(void* data, uint32_t offset, uint32_t size);

    /**
     * @brief Get a part of the buffer
     * @param offset First element to get
     * @param size Number of elements to get
     * @return Pointer to the data (must be deallocated after use)
    **/
    void* getData(uint32_t offset, uint32_t size);

    /**
     * @brief Set a part of the buffer to zero
     * @param offset First element to modify
     * @param size Number of elements to modify (size * stride must be a multiple of 4 bytes)
     */
    void clearData(uint32_t offset, uint32_t size);

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
     * @param target Buffer target
     * @param binding Binding of the buffer in the shader(s)
     * @param stride Size of an element in the buffer (in bytes)
    **/
    ShaderBuffer(BufferTarget target, int binding, int stride);

    /**
     * @brief Create a new shader buffer referencing an existing buffer
     * @param buffer Buffer to reference
     * @param target Target of the new buffer
     * @param binding Binding of the buffer in the shader(s)
     * @param stride Size of an element in the buffer (in bytes)
    **/
    ShaderBuffer(Buffer buffer, BufferTarget target, int binding, int stride);

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
     * @param stride Size of an element in the buffer (in bytes)
    **/
    explicit VerticesBuffer(int stride) : Buffer(BufferTarget::vertices, stride) {}

    /**
     * @brief Create a new vertices buffer referencing an existing buffer
     * @param buffer Buffer to reference
     * @param stride Size of an element in the buffer (in bytes)
    **/
    VerticesBuffer(Buffer buffer, int stride) : Buffer(buffer, BufferTarget::vertices, stride) {}

};



class InstancesBuffer : public Buffer {

public:
    /**
     * @brief Create a new instances buffer
     * @param stride Size of an element in the buffer (in bytes)
    **/
    explicit InstancesBuffer(int stride) : Buffer(BufferTarget::instances, stride) {}

    /**
     * @brief Create a new instances buffer referencing an existing buffer
     * @param buffer Buffer to reference
     * @param stride Size of an element in the buffer (in bytes)
    **/
    InstancesBuffer(Buffer buffer, int stride) : Buffer(buffer, BufferTarget::instances, stride) {}

};



class IndicesBuffer : public Buffer {

public:
    /**
     * @brief Create a new indices buffer
     * @param type Data type of the indices in the buffer
    **/
    explicit IndicesBuffer(IndexType type) : Buffer(BufferTarget::indices, indexSizes[(int)type]) {}

    /**
     * @brief Create a new indices buffer referencing an existing buffer
     * @param buffer Buffer to reference
     * @param stride Size of an element in the buffer (in bytes)
    **/
    IndicesBuffer(Buffer buffer, int stride) : Buffer(buffer, BufferTarget::indices, stride) {}

};



class StorageBuffer : public ShaderBuffer {

public:
    /**
     * @brief Create a new storage buffer
     * @param binding Binding of the buffer in the shader(s)
     * @param stride Size of an element in the buffer (in bytes)
    **/
    StorageBuffer(int binding, int stride) : ShaderBuffer(BufferTarget::storage, binding, stride) {}

    /**
     * @brief Create a new storage buffer referencing an existing buffer
     * @param buffer Buffer to reference
     * @param binding Binding of the buffer in the shader(s)
     * @param stride Size of an element in the buffer (in bytes)
    **/
    StorageBuffer(Buffer buffer, int binding, int stride) : ShaderBuffer(buffer, BufferTarget::storage, binding, stride) {}

};



class CountersBuffer : public ShaderBuffer {

public:
    /**
     * @brief Create a new counters buffer
     * @param binding Binding of the buffer in the shader(s)
     * @param stride Size of an element in the buffer (in bytes)
    **/
    CountersBuffer(int binding, int stride) : ShaderBuffer(BufferTarget::counters, binding, stride) {}

    /**
     * @brief Create a new counters buffer referencing an existing buffer
     * @param buffer Buffer to reference
     * @param binding Binding of the buffer in the shader(s)
     * @param stride Size of an element in the buffer (in bytes)
    **/
    CountersBuffer(Buffer buffer, int binding, int stride) : ShaderBuffer(buffer, BufferTarget::counters, binding, stride) {}

};



class IndirectDrawBuffer : public Buffer {

public:
    /**
     * @brief Create a new indirect draw buffer
     * @param stride Size of an element in the buffer (in bytes)
    **/
    explicit IndirectDrawBuffer(int stride) : Buffer(BufferTarget::indirectDraw, stride) {}

    /**
     * @brief Create a new indirect draw buffer referencing an existing buffer
     * @param buffer Buffer to reference
     * @param stride Size of an element in the buffer (in bytes)
    **/
    IndirectDrawBuffer(Buffer buffer, int stride) : Buffer(buffer, BufferTarget::indirectDraw, stride) {}

};



class IndirectDispatchBuffer : public Buffer {

public:
    /**
     * @brief Create a new indirect dispatch buffer
     * @param stride Size of an element in the buffer (in bytes)
    **/
    explicit IndirectDispatchBuffer(int stride) : Buffer(BufferTarget::indirectDispatch, stride) {}

    /**
     * @brief Create a new indirect dispatch buffer referencing an existing buffer
     * @param buffer Buffer to reference
     * @param stride Size of an element in the buffer (in bytes)
    **/
    IndirectDispatchBuffer(Buffer buffer, int stride) : Buffer(buffer, BufferTarget::indirectDispatch, stride) {}

};



class ParametersBuffer : public Buffer {

public:
    /**
     * @brief Create a new parameters buffer
     * @param stride Size of an element in the buffer (in bytes)
    **/
    explicit ParametersBuffer(int stride) : Buffer(BufferTarget::parameters, stride) {}

    /**
     * @brief Create a new parameters buffer referencing an existing buffer
     * @param buffer Buffer to reference
     * @param stride Size of an element in the buffer (in bytes)
    **/
    ParametersBuffer(Buffer buffer, int stride) : Buffer(buffer, BufferTarget::parameters, stride) {}

};



#endif // BUFFER_H