#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include <glad/glad.h>

#include "GLObjects/Buffer.hpp"



enum class AttributeType : GLenum {
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


enum class IndexType : GLenum {
    uint8 = GL_UNSIGNED_BYTE,
    uint16 = GL_UNSIGNED_SHORT,
    uint32 = GL_UNSIGNED_INT,
};



class VertexArray {

public:
    bool indexed = false;
    IndexType indexType;

    /**
     * @brief Create a new vertex array object (VAO)
    **/
    VertexArray();

    /**
     * @brief Add a vertices attribute from a vertices buffer to the array
     * @param index Index of the attribute
     * @param buffer The vertices buffer the attribute is stored in
     * @param type Data type of the attribute
     * @param size Number of components of the attribute
     * @param stride Size of the attribute (in bytes)
     * @param offset Offset of the attribute in each vertex in the buffer (in bytes)
     * @param normalize Whether to normalize the attribute in case of a conversion to a float
    **/
    void setVerticesAttribute(int index, VerticesBuffer buffer, AttributeType type, int size, int stride, int offset, bool normalize = false);

    /**
     * @brief Remove a vertices attribute
     * @param index Index of the attribute
    **/
    void removeVerticesAttribute(int index);

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
     * @brief Stop using the array for future OpenGL calls
    **/
    void unbind();

    /**
     * @brief Delete the array
    **/
    void dispose();

private:
    GLuint array;

};


#endif // VERTEX_ARRAY_H