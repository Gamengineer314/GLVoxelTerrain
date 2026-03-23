#ifndef SHADER_H
#define SHADER_H

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GLObjects/Buffer.hpp"

namespace gl {

enum class ShaderType : GLenum {
    vertex = GL_VERTEX_SHADER,
    fragment = GL_FRAGMENT_SHADER,
    geometry = GL_GEOMETRY_SHADER,
    compute = GL_COMPUTE_SHADER,
    tessControl = GL_TESS_CONTROL_SHADER,
    tessEvaluation = GL_TESS_EVALUATION_SHADER
};


class Shader {
public:
    Shader() : program(glCreateProgram()) {}

    ~Shader() {
        glDeleteProgram(program);
    }

    Shader(Shader&& other) : program(other.program) {
        other.program = 0;
    }

    Shader& operator=(Shader other) {
        std::swap(program, other.program);
        return *this;
    }

    Shader(Shader const&) = delete;

    /**
     * @brief Use the shader for future OpenGL calls
    **/
    void use() const {
        glUseProgram(program);
    }

    /**
     * @brief Get the OpenGL program object
    **/
    uint32_t id() const {
        return program;
    }

protected:
    GLuint program;

    /**
     * @brief Attach a new shader to the program
     * @param path Path to the shader file
     * @param type Shader type
    **/
    void attachShader(char const* path, ShaderType type) const;
};


class GraphicsShader : public Shader {
public:
    /**
     * @brief Create a new shader for rendering
     * @param vertexPath Path to the vertex shader file
     * @param fragmentPath Path to the fragment shader file
    **/
    GraphicsShader(char const* vertexPath, char const* fragmentPath) : Shader() {
        attachShader(vertexPath, ShaderType::vertex);
        attachShader(fragmentPath, ShaderType::fragment);
        glLinkProgram(program);
    }
};


class ComputeShader : public Shader {
public:
    /**
     * @brief Create a new compute shader
     * @param path Path to the compute shader file
    **/
    explicit ComputeShader(char const* path) : Shader() {
        attachShader(path, ShaderType::compute);
        glLinkProgram(program);
    }
};


class Uniform {
public:
    /**
     * @brief Get a uniform
     * @param shader The uniform's shader
     * @param name Name of the uniform in the shader
    **/
    Uniform(Shader const& shader, char const* name) :
        location(glGetUniformLocation(shader.id(), name)) {}

    /**
     * @brief Set the value of the matrix uniform
     * @param shader The uniform's shader
     * @param value The value to set the uniform to
    **/
    void setValue(Shader const& shader, glm::mat4 const& value) {
        glProgramUniformMatrix4fv(shader.id(), location, 1, GL_FALSE, value_ptr(value));
    }

    /**
     * @brief Set the value of the vector uniform
     * @param shader The uniform's shader
     * @param value The value to set the uniform to
    **/
    void setValue(Shader const& shader, glm::vec3 const& value) {
        glProgramUniform3fv(shader.id(), location, 1, value_ptr(value));
    }

    /**
     * @brief Set the value of the vector uniform
     * @param shader The uniform's shader
     * @param value The value to set the uniform to
    **/
    void setValue(Shader const& shader, glm::vec4 const& value) {
        glProgramUniform4fv(shader.id(), location, 1, value_ptr(value));
    }

    /**
     * @brief Set the value of the vector uniform
     * @param shader The uniform's shader
     * @param value The value to set the uniform to
    **/
    void setValue(Shader const& shader, float value) {
        glProgramUniform1f(shader.id(), location, value);
    }

    /**
     * @brief Get the OpenGL uniform location
    **/
    int32_t id() {
        return location;
    }

private:
    GLint location;
};

}

#endif