#ifndef BUFFER_H
#define BUFFER_H

#include <cstdint>
#include <memory>
#include <glad/glad.h>

using namespace std;


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


enum class BufferType : GLenum {
    vertices = GL_ARRAY_BUFFER,
    indices = GL_ELEMENT_ARRAY_BUFFER,
    indirectDraw = GL_DRAW_INDIRECT_BUFFER,
    indirectDispatch = GL_DISPATCH_INDIRECT_BUFFER,
    parameters = GL_PARAMETER_BUFFER,
    pixelPack = GL_PIXEL_PACK_BUFFER,
    pixelUnpack = GL_PIXEL_UNPACK_BUFFER,
    query = GL_QUERY_BUFFER,
    texture = GL_TEXTURE_BUFFER,
    copyRead = GL_COPY_READ_BUFFER,
    copyWrite = GL_COPY_WRITE_BUFFER,
    storage = GL_SHADER_STORAGE_BUFFER,
    uniform = GL_UNIFORM_BUFFER,
    counters = GL_ATOMIC_COUNTER_BUFFER,
    transformFeedback = GL_TRANSFORM_FEEDBACK_BUFFER
};


enum class ShaderBufferType : GLenum {
    storage = GL_SHADER_STORAGE_BUFFER,
    uniform = GL_UNIFORM_BUFFER,
    counters = GL_ATOMIC_COUNTER_BUFFER,
    transformFeedback = GL_TRANSFORM_FEEDBACK_BUFFER
};


class Buffer {

public:

    Buffer() {
        glCreateBuffers(1, &buffer);
    }

    ~Buffer() {
        glDeleteBuffers(1, &buffer);
    }

    Buffer(Buffer&& other) : buffer(other.buffer) {
        other.buffer = 0;
    }

    Buffer& operator=(Buffer&& other) {
        if (this != &other) {
            buffer = other.buffer;
            other.buffer = 0;
        }
        return *this;
    }

    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

    /**
     * @brief Set the data of the buffer. No more calls to setDataUnique() can then be made.
     * @param data Data to store in the buffer
     * @param n Number of elements to get
     * @param usage Usage of the buffer
    **/
    template<typename T> void setData(T* data, uint32_t n, BufferUsage usage) const {
        glNamedBufferData(buffer, n * sizeof(T), data, (GLenum)usage);
    }

    /**
     * @brief Set the data of the buffer. No more calls to setData() or setDataUnique() can then be made.
     * @param data Data to store in the buffer
     * @param n Number of elements to get
     * @param usage Usage of the buffer
    **/
    template<typename T> void setDataUnique(T* data, uint32_t n, UniqueBufferUsage usage) const {
        glNamedBufferStorage(buffer, n * sizeof(T), data, (GLenum)usage);
    }

    /**
     * @brief Modify a part of the buffer without resizing it
     * @param data New data to store in the buffer
     * @param n Number of elements to get
     * @param start First element to get
    **/
    template<typename T> void modifyData(T* data, uint32_t n, uint32_t start = 0) const {
        glNamedBufferSubData(buffer, start * sizeof(T), n * sizeof(T), data);
    }

    /**
     * @brief Get a part of the buffer
     * @param n Number of elements to get
     * @param start First element to get
     * @return Data in the buffer
    **/
    template<typename T> unique_ptr<T[]> getData(uint32_t n, uint32_t start = 0) const {
        unique_ptr<T[]> data(new T[n]);
        glGetNamedBufferSubData(buffer, start * sizeof(T), n * sizeof(T), data.get());
        return data;
    }

    /**
     * @brief Set the data of the buffer to zero
    **/
    void clearData() const {
        glClearNamedBufferData(buffer, GL_R32UI, GL_RED_INTEGER, GL_UNSIGNED_INT, nullptr);
    }

    /**
     * @brief Set a part of the data of the buffer to zero
     * @param size Size of the part of the buffer to clear (must be a multiple of 4 bytes)
     * @param offset Start of the part of the buffer to clear
    **/
    void clearData(uint32_t size, uint32_t offset = 0) const {
        glClearNamedBufferSubData(buffer, GL_R32UI, offset, size, GL_RED_INTEGER, GL_UNSIGNED_INT, nullptr);
    }

    /**
     * @brief Use the buffer for future OpenGL calls
     * @param type Buffer type
    **/
    void use(BufferType type) const {
        glBindBuffer((GLenum)type, buffer);
    }

    /**
     * @brief Use the buffer for future OpenGL calls
     * @param type Buffer type
     * @param index Buffer index
    **/
    void use(ShaderBufferType type, uint32_t index) const {
        glBindBufferBase((GLenum)type, index, buffer);
    }

    /**
     * @brief Get the OpenGL buffer object
    **/
    uint32_t id() const {
        return buffer;
    }

private:

    GLuint buffer;
    
};


struct ShaderBuffer {
    const Buffer* buffer;
    ShaderBufferType type;
    uint32_t index;

    ShaderBuffer(const Buffer& buffer, ShaderBufferType type, uint32_t index) :
        buffer(&buffer), type(type), index(index) {}
};


struct IndirectDrawArgs {
    uint32_t vertexCount;
    uint32_t instanceCount;
    uint32_t firstVertex;
    uint32_t baseInstance;

    IndirectDrawArgs() = default;
    IndirectDrawArgs(uint32_t vertexCount, uint32_t instanceCount = 1, uint32_t firstVertex = 0, uint32_t baseInstance = 0) :
        vertexCount(vertexCount), instanceCount(instanceCount), firstVertex(firstVertex), baseInstance(baseInstance) {}
};


struct IndirectIndexedDrawArgs {
    uint32_t vertexCount;
    uint32_t instanceCount;
    uint32_t firstVertex;
    int32_t baseVertex;
    uint32_t baseInstance;

    IndirectIndexedDrawArgs() = default;
    IndirectIndexedDrawArgs(uint32_t vertexCount, uint32_t instanceCount = 1, uint32_t firstVertex = 0, uint32_t baseVertex = 0, uint32_t baseInstance = 0) :
        vertexCount(vertexCount), instanceCount(instanceCount), firstVertex(firstVertex), baseVertex(baseVertex), baseInstance(baseInstance) {}
};


struct IndirectDispatchArgs {
    uint32_t numGroupsX;
    uint32_t numGroupsY;
    uint32_t numGroupsZ;

    IndirectDispatchArgs() = default;
    IndirectDispatchArgs(uint32_t numGroupsX = 1, uint32_t numGroupsY = 1, uint32_t numGroupsZ = 1) :
        numGroupsX(numGroupsX), numGroupsY(numGroupsY), numGroupsZ(numGroupsZ) {}
};


#endif // BUFFER_H