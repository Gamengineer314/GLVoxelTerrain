#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

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
    friend class Uniform;

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

}

#endif