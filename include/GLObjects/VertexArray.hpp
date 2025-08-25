#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include <glad/glad.h>

#include "GLObjects/Buffer.hpp"


enum class IntAttributeType : GLenum {
    int8 = GL_BYTE,
    uint8 = GL_UNSIGNED_BYTE,
    int16 = GL_SHORT,
    uint16 = GL_UNSIGNED_SHORT,
    int32 = GL_INT,
    uint32 = GL_UNSIGNED_INT
};


enum class FloatAttributeType : GLenum {
    float16 = GL_HALF_FLOAT,
    float32 = GL_FLOAT,
    float64 = GL_DOUBLE,
    fixed32 = GL_FIXED,
    uint_10f_11f_11f = GL_UNSIGNED_INT_10F_11F_11F_REV
};


enum class ToFloatAttributeType : GLenum {
    int8 = GL_BYTE,
    uint8 = GL_UNSIGNED_BYTE,
    int16 = GL_SHORT,
    uint16 = GL_UNSIGNED_SHORT,
    int32 = GL_INT,
    uint32 = GL_UNSIGNED_INT,
    int_2_10_10_10 = GL_INT_2_10_10_10_REV,
    uint_2_10_10_10 = GL_UNSIGNED_INT_2_10_10_10_REV
};


enum class IndexType : GLenum {
    uint8 = GL_UNSIGNED_BYTE,
    uint16 = GL_UNSIGNED_SHORT,
    uint32 = GL_UNSIGNED_INT,
    none = 0
};


class VertexArray {

public:

    VertexArray() {
        glCreateVertexArrays(1, &array);
    }

    ~VertexArray() {
        glDeleteVertexArrays(1, &array);
    }

    VertexArray(VertexArray&& other) : array(other.array) {
        other.array = 0;
    }

    VertexArray& operator=(VertexArray&& other) {
        if (this != &other) {
            array = other.array;
            other.array = 0;
        }
        return *this;
    }

    VertexArray(const VertexArray&) = delete;
    VertexArray& operator=(const VertexArray&) = delete;

    /**
     * @brief Add or set a buffer in the array
     * @param index Index of the buffer
     * @param buffer The buffer
     * @param stride Size of an element in the buffer
     * @param start First element to use
     * @param instances 0 for a vertex buffer, 1 for an instance buffer, > 1 to specify the number of instances that share the same attribute
    **/
    void setBuffer(uint32_t index, const Buffer& buffer, uint32_t stride, uint32_t start = 0, uint32_t instances = 0) const {
        glVertexArrayVertexBuffer(array, index, buffer.id(), start * stride, stride);
        if (instances > 0) glVertexArrayBindingDivisor(array, index, instances);
    }

    /**
     * @brief Set the buffer where an attribute is stored
     * @param attribute Index of the attribute
     * @param buffer Index of the buffer
    **/
    void setAttributeBuffer(uint32_t attribute, uint32_t buffer) const {
        glVertexArrayAttribBinding(array, attribute, buffer);
    }

    /**
     * @brief Set the format of an integer attribute in the array
     * @param index Index of the attribute
     * @param type Attribute type
     * @param components Number of components
     * @param offset Offset of the attribute in its buffer
     * @param enabled Whether the attribute is enabled
    **/
    void setAttributeFormat(uint32_t index, IntAttributeType type, uint32_t components = 1, uint32_t offset = 0, bool enabled = true) const {
        glVertexArrayAttribIFormat(array, index, components, (GLenum)type, offset);
        if (enabled) enableAttribute(index);
    }

    /**
     * @brief Set the format of a floating point attribute in the array
     * @param index Index of the attribute
     * @param type Attribute type
     * @param components Number of components
     * @param offset Offset of the attribute in its buffer
     * @param enabled Whether the attribute is enabled
    **/
    void setAttributeFormat(uint32_t index, FloatAttributeType type, uint32_t components = 1, uint32_t offset = 0, bool enabled = true) const {
        if (type == FloatAttributeType::float64) glVertexArrayAttribLFormat(array, index, components, (GLenum)type, offset);
        else glVertexArrayAttribFormat(array, index, components, (GLenum)type, GL_FALSE, offset);
        if (enabled) enableAttribute(index);
    }

    /**
     * @brief Set the format of a floating point attribute from integer data in the array
     * @param index Index of the attribute
     * @param type Attribute type
     * @param normalize Whether to normalize the attribute when converting to a float
     * @param components Number of components
     * @param offset Offset of the attribute in its buffer
     * @param enabled Whether the attribute is enabled
    **/
    void setAttributeFormat(uint32_t index, ToFloatAttributeType type, bool normalize, uint32_t components = 1, uint32_t offset = 0, bool enabled = true) const {
        glVertexArrayAttribFormat(array, index, components, (GLenum)type, GL_FALSE, offset);
        if (enabled) enableAttribute(index);
    }

    /**
     * @brief Enable an attribute in the array
     * @param index Index of the attribute
    **/
    void enableAttribute(uint32_t index) const {
        glEnableVertexArrayAttrib(array, index);
    }

    /**
     * @brief Disable an attribute in the array
     * @param index Index of the attribute
    **/
    void disableAttribute(uint32_t index) const {
        glDisableVertexArrayAttrib(array, index);
    }

    /**
     * @brief Add an indices buffer to the array
     * @param buffer The buffer to add
     * @param indexType Data type of the indices
    **/
    void setIndices(const Buffer& buffer, IndexType indexType) {
        glVertexArrayElementBuffer(array, buffer.id());
        this->indexType = indexType;
    }

    /**
     * @brief Remove the indices buffer
    **/
    void removeIndices() {
        glVertexArrayElementBuffer(array, 0);
        indexType = IndexType::none;
    }

    /**
     * @brief Use the array for future OpenGL calls
    **/
    void use() const {
        glBindVertexArray(array);
    }

    /**
     * @brief Get the OpenGL buffer object
    **/
    uint32_t id() const {
        return array;
    }

    /**
     * @brief Index type if the array is indexed, IndexType::none otherwise
    **/
    IndexType indices() const {
        return indexType;
    }

private:

    GLuint array;
    IndexType indexType = IndexType::none;

};


#endif // VERTEX_ARRAY_H