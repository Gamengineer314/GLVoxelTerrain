#include "GLObjects/Shader.hpp"

#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GLObjects/Buffer.hpp"

using namespace std;
using namespace glm;

static GLuint usedProgram = (GLuint)-1;



Shader::Shader() :
    program(glCreateProgram()) {
}


void Shader::attachShader(const char* path, ShaderType type) {
    // Read file
    ifstream file = ifstream(path);
    stringstream stream;
    stream << file.rdbuf();
    string str = stream.str();
    const char* shaderCode = str.c_str();
    file.close();

    // Compile and attach shader
    GLuint shader = glCreateShader((GLenum)type);
    glShaderSource(shader, 1, &shaderCode, nullptr);
    glCompileShader(shader);
    glAttachShader(program, shader);
    glDeleteShader(shader);
}


void Shader::use() {
    if (usedProgram != program) {
        glUseProgram(program);
        usedProgram = program;
    }
}


Shader::Uniform Shader::getUniform(const char* name) {
    return Uniform(*this, name);
}


void Shader::dispose() {
    glDeleteProgram(program);
}



Shader::Uniform::Uniform(Shader& shader, const char* name) :
    shader(shader),
    location(glGetUniformLocation(shader.program, name)) {
}


void Shader::Uniform::setValue(mat4& value) {
    shader.use();
    glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(value));
}

void Shader::Uniform::setValue(vec3& value) {
    shader.use();
    glUniform3fv(location, 1, value_ptr(value));
}

void Shader::Uniform::setValue(vec4& value) {
    shader.use();
    glUniform4fv(location, 1, value_ptr(value));
}

void Shader::Uniform::setValue(float value) {
    shader.use();
    glUniform1f(location, value);
}



GraphicsShader::GraphicsShader(const char* vertexPath, const char* fragmentPath) :
    Shader() {
    attachShader(vertexPath, ShaderType::vertex);
    attachShader(fragmentPath, ShaderType::fragment);
    glLinkProgram(program);
}



ComputeShader::ComputeShader(const char* path) :
    Shader() {
    attachShader(path, ShaderType::compute);
    glLinkProgram(program);
}


void ComputeShader::dispatch(int numGroupsX, int numGroupsY, int numGroupsZ, ShaderBuffer* shaderBuffers, int numShaderBuffers) {
    use();
    if (shaderBuffers != nullptr) {
        for (int i = 0; i < numShaderBuffers; i++) {
            shaderBuffers[i].shaderBind();
        }
    }
    glDispatchCompute(numGroupsX, numGroupsY, numGroupsZ);
}


void ComputeShader::dispatchIndirect(IndirectDispatchBuffer commands, int commandIndex, ShaderBuffer* shaderBuffers, int numShaderBuffers) {
    use();
    commands.bind();
    if (shaderBuffers != nullptr) {
        for (int i = 0; i < numShaderBuffers; i++) {
            shaderBuffers[i].shaderBind();
        }
    }
    glDispatchComputeIndirect((GLintptr)(commandIndex * commands.stride));
}