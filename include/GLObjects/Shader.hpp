#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "GLObjects/Buffer.hpp"

using namespace glm;


class Shader {

public:
    class Uniform {

    public:
        /**
         * @brief Create a new uniform
         * @param shader Shader the uniform is in
         * @param name Name of the uniform in the shader
        **/
        Uniform(Shader& shader, const char* name);

        /**
         * @brief Set the value of the matrix uniform
         * @param value The value to set the uniform to
        **/
        void setValue(mat4& value);

        /**
         * @brief Set the value of the vector uniform
         * @param value The value to set the uniform to
        **/
        void setValue(vec3& value);

        /**
         * @brief Set the value of the vector uniform
         * @param value The value to set the uniform to
        **/
        void setValue(float value);

    private:
        Shader& shader;
        GLint location;

    };

    /**
     * @brief Create a new shader
     * @param vertexPath Path to the vertex shader file
     * @param fragmentPath Path to the fragment shader file
    **/
    Shader(const char* vertexPath, const char* fragmentPath);

    /**
     * @brief Use the shader for future glDraw*() calls
    **/
    void use();

    /**
     * @brief Get a uniform by its name
     * @param name Name of the uniform in the shader
    **/
    Uniform getUniform(const char* name);

    /**
     * @brief Delete the shader
    **/
    void dispose();

private:
    GLuint program;

};


#endif // SHADER_H