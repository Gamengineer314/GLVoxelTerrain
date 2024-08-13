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
    fuint_10_11_11 = GL_UNSIGNED_INT_10F_11F_11F_REV
};


enum class ToFloatAttributeType : GLenum {
    int8 = GL_BYTE,
    uint8 = GL_UNSIGNED_BYTE,
    int16 = GL_SHORT,
    uint16 = GL_UNSIGNED_SHORT,
    int32 = GL_INT,
    uint32 = GL_UNSIGNED_INT,
    float16 = GL_HALF_FLOAT,
    float32 = GL_FLOAT,
    float64 = GL_DOUBLE,
    fixed32 = GL_FIXED,
    int_2_10_10_10 = GL_INT_2_10_10_10_REV,
    uint_2_10_10_10 = GL_UNSIGNED_INT_2_10_10_10_REV,
    fuint_10_11_11 = GL_UNSIGNED_INT_10F_11F_11F_REV
};



class VertexArray {

public:
    bool indexed = false;
    int indexType;

    /**
     * @brief Create a new vertex array object (VAO)
    **/
    VertexArray();

    /**
     * @brief Add a vertices float attribute from a vertices buffer
     * @param index Index of the attribute
     * @param buffer The vertices buffer the attribute is stored in
     * @param type Data type of the attribute in the buffer
     * @param size Number of components of the attribute
     * @param offset Offset of the attribute in each vertex in the buffer (in bytes)
    **/
    void setVerticesFloat(int index, VerticesBuffer buffer, FloatAttributeType type, int size, int offset);

    /**
     * @brief Add a vertices double attribute from a vertices buffer
     * @param index Index of the attribute
     * @param buffer The vertices buffer the attribute is stored in
     * @param size Number of components of the attribute
     * @param offset Offset of the attribute in each vertex in the buffer (in bytes)
    **/
    void setVerticesDouble(int index, VerticesBuffer buffer, int size, int offset);

    /**
     * @brief Add a vertices integer attribute from a vertices buffer
     * @param index Index of the attribute
     * @param buffer The vertices buffer the attribute is stored in
     * @param type Data type of the attribute in the buffer
     * @param size Number of components of the attribute
     * @param offset Offset of the attribute in each vertex in the buffer (in bytes)
    **/
    void setVerticesInt(int index, VerticesBuffer buffer, IntAttributeType type, int size, int offset);

    /**
     * @brief Add a vertices float attribute converted from a vertices buffer containing any type
     * @param index Index of the attribute
     * @param buffer The vertices buffer the attribute is stored in
     * @param type Data type of the attribute in the buffer
     * @param size Number of components of the attribute
     * @param offset Offset of the attribute in each vertex in the buffer (in bytes)
     * @param normalize Whether to normalize the attribute when converting to a float
    **/
    void setVerticesToFloat(int index, VerticesBuffer buffer, ToFloatAttributeType type, int size, int offset, bool normalize);

    /**
     * @brief Add an instances float attribute from a vertices buffer
     * @param index Index of the attribute
     * @param buffer The vertices buffer the attribute is stored in
     * @param type Data type of the attribute in the buffer
     * @param size Number of components of the attribute
     * @param offset Offset of the attribute in each vertex in the buffer (in bytes)
     * @param divisor Number of instances that share the same attribute
    **/
    void setInstancesFloat(int index, InstancesBuffer buffer, FloatAttributeType type, int size, int offset, int divisor = 1);

    /**
     * @brief Add a vertices double attribute from a vertices buffer
     * @param index Index of the attribute
     * @param buffer The vertices buffer the attribute is stored in
     * @param size Number of components of the attribute
     * @param offset Offset of the attribute in each vertex in the buffer (in bytes)
     * @param divisor Number of instances that share the same attribute
    **/
    void setInstancesDouble(int index, InstancesBuffer buffer, int size, int offset, int divisor = 1);

    /**
     * @brief Add a vertices integer attribute from a vertices buffer
     * @param index Index of the attribute
     * @param buffer The vertices buffer the attribute is stored in
     * @param type Data type of the attribute in the buffer
     * @param size Number of components of the attribute
     * @param offset Offset of the attribute in each vertex in the buffer (in bytes)
     * @param divisor Number of instances that share the same attribute
    **/
    void setInstancesInt(int index, InstancesBuffer buffer, IntAttributeType type, int size, int offset, int divisor = 1);

    /**
     * @brief Add a vertices float attribute converted from a vertices buffer containing any type
     * @param index Index of the attribute
     * @param buffer The vertices buffer the attribute is stored in
     * @param type Data type of the attribute in the buffer
     * @param size Number of components of the attribute
     * @param offset Offset of the attribute in each vertex in the buffer (in bytes)
     * @param normalize Whether to normalize the attribute when converting to a float
     * @param divisor Number of instances that share the same attribute
    **/
    void setInstancesToFloat(int index, InstancesBuffer buffer, ToFloatAttributeType type, int size, int offset, bool normalize, int divisor = 1);

    /**
     * @brief Remove a vertices attribute
     * @param index Index of the attribute
    **/
    void removeVerticesAttribute(int index);

    /**
     * @brief Remove an instances attribute
     * @param index Index of the attribute
    **/
    void removeInstancesAttribute(int index);

    /**
     * @brief Add an indices buffer to the array
     * @param buffer The indices buffer to add
     * @param indexType Data type of the indices
    **/
    void setIndices(IndicesBuffer buffer, IndexType indexType);

    /**
     * @brief Remove the indices buffer
    **/
    void removeIndices();

    /**
     * @brief Use the array for future OpenGL calls
    **/
    void bind();

    /**
     * @brief Delete the array
    **/
    void dispose();

private:
    GLuint array;

};


#endif // VERTEX_ARRAY_H