#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "GLObjects/Buffer.hpp"

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
        void setValue(vec4& value);

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
     * @brief Create a new empty shader program
    **/
    Shader();

    /**
     * @brief Use the shader for future OpenGL calls
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

protected:
    GLuint program;

    /**
     * @brief Attach a new shader to the program
     * @param path Path to the shader file
     * @param type Shader type
    **/
    void attachShader(const char* path, ShaderType type);

};



class GraphicsShader : public Shader {

public:
    /**
     * @brief Create a new shader for rendering
     * @param vertexPath Path to the vertex shader file
     * @param fragmentPath Path to the fragment shader file
    **/
    GraphicsShader(const char* vertexPath, const char* fragmentPath);

};



class ComputeShader : public Shader {

public:
    /**
     * @brief Create a new compute shader
     * @param path Path to the compute shader file
    **/
    explicit ComputeShader(const char* path);

    /**
     * @brief Dispatch the compute shader
     * @param numGroupsX Number of work groups in the X dimension
     * @param numGroupsY Number of work groups in the Y dimension
     * @param numGroupsZ Number of work groups in the Z dimension
     * @param shaderBuffers Shader buffers to use
     * @param numShaderBuffers Number of elements in shaderBuffers
    **/
    void dispatch(int numGroupsX, int numGroupsY, int numGroupsZ, ShaderBuffer* shaderBuffers = nullptr, int numShaderBuffers = 0);

    /**
     * @brief Dispatch the compute shader with commands provided as a buffer
     * @param commands Indirect dispatch command(s)
     * @param commandIndex Index of the command to use
     * @param barrier Memory barrier to use
     * @param shaderBuffers Shader buffers to use
     * @param numShaderBuffers Number of elements in shaderBuffers
    **/
    void dispatchIndirect(IndirectDispatchBuffer commands, int commandIndex, ShaderBuffer* shaderBuffers = nullptr, int numShaderBuffers = 0);

};



#endif // SHADER_H