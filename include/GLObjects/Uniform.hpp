#ifndef UNIFORM_H
#define UNIFORM_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GLObjects/Shader.hpp"

namespace gl {

class Uniform {
public:
    /**
     * @brief Get a uniform
     * @param shader The uniform's shader
     * @param name Name of the uniform in the shader
    **/
    Uniform(Shader const& shader, char const* name) :
        location(glGetUniformLocation(shader.program, name)) {}

    /**
     * @brief Set the value of the matrix uniform
     * @param shader The uniform's shader
     * @param value The value to set the uniform to
    **/
    void setValue(Shader const& shader, glm::mat4 const& value) {
        glProgramUniformMatrix4fv(shader.program, location, 1, GL_FALSE, value_ptr(value));
    }

    /**
     * @brief Set the value of the vector uniform
     * @param shader The uniform's shader
     * @param value The value to set the uniform to
    **/
    void setValue(Shader const& shader, glm::vec3 const& value) {
        glProgramUniform3fv(shader.program, location, 1, value_ptr(value));
    }

    /**
     * @brief Set the value of the vector uniform
     * @param shader The uniform's shader
     * @param value The value to set the uniform to
    **/
    void setValue(Shader const& shader, glm::vec4 const& value) {
        glProgramUniform4fv(shader.program, location, 1, value_ptr(value));
    }

    /**
     * @brief Set the value of the vector uniform
     * @param shader The uniform's shader
     * @param value The value to set the uniform to
    **/
    void setValue(Shader const& shader, float value) {
        glProgramUniform1f(shader.program, location, value);
    }

private:
    GLint location;
};

}

#endif