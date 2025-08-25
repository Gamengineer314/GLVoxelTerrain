#ifndef SHADER_H
#define SHADER_H

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GLObjects/Buffer.hpp"

using namespace std;
using namespace glm;


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

    Shader& operator=(Shader&& other) {
        if (this != &other) {
            program = other.program;
            other.program = 0;
            other.shaderBuffers = move(shaderBuffers);
        }
        return *this;
    }

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    /**
     * @brief Add or set a buffer in the shader
     * @param index Index of the buffer
     * @param type Buffer type
     * @param buffer The buffer
    **/
    void setBuffer(uint32_t index, ShaderBufferType type, const Buffer& buffer);

    /**
     * @brief Get the buffers in the shader
    **/
    const vector<ShaderBuffer>& buffers() const {
        return shaderBuffers;
    }

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
    vector<ShaderBuffer> shaderBuffers;

    /**
     * @brief Attach a new shader to the program
     * @param path Path to the shader file
     * @param type Shader type
    **/
    void attachShader(const char* path, ShaderType type) const;

};


class GraphicsShader : public Shader {

public:

    /**
     * @brief Create a new shader for rendering
     * @param vertexPath Path to the vertex shader file
     * @param fragmentPath Path to the fragment shader file
    **/
    GraphicsShader(const char* vertexPath, const char* fragmentPath) : Shader() {
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
    explicit ComputeShader(const char* path) : Shader() {
        attachShader(path, ShaderType::compute);
        glLinkProgram(program);
    }

};


class Uniform {

public:
    /**
     * @brief Get a uniform in a shader
     * @param shader The shader
     * @param name Name of the uniform in the shader
    **/
    Uniform(Shader& shader, const char* name) : 
        shader(shader), location(glGetUniformLocation(shader.id(), name)) {}

    /**
     * @brief Set the value of the matrix uniform
     * @param value The value to set the uniform to
    **/
    void setValue(mat4& value) {
        glProgramUniformMatrix4fv(shader.id(), location, 1, GL_FALSE, value_ptr(value));
    }

    /**
     * @brief Set the value of the vector uniform
     * @param value The value to set the uniform to
    **/
    void setValue(vec3& value) {
        glProgramUniform3fv(shader.id(), location, 1, value_ptr(value));
    }

    /**
     * @brief Set the value of the vector uniform
     * @param value The value to set the uniform to
    **/
    void setValue(vec4& value) {
        glProgramUniform4fv(shader.id(), location, 1, value_ptr(value));
    }

    /**
     * @brief Set the value of the vector uniform
     * @param value The value to set the uniform to
    **/
    void setValue(float value) {
        glProgramUniform1f(shader.id(), location, value);
    }

    /**
     * @brief Get the OpenGL uniform location
    **/
    int32_t id() {
        return location;
    }

private:
    Shader& shader;
    GLint location;

};


#endif // SHADER_H