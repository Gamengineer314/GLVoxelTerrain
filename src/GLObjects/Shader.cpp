#include "GLObjects/Shader.hpp"

#include <fstream>
#include <sstream>

using namespace std;


void Shader::setBuffer(uint32_t index, ShaderBufferType type, const Buffer& buffer) {
    for (ShaderBuffer& shaderBuffer : shaderBuffers) {
        if (shaderBuffer.index == index && shaderBuffer.type == type) {
            shaderBuffer.buffer = &buffer;
            return;
        }
    }
    shaderBuffers.push_back(ShaderBuffer(buffer, type, index));
}


void Shader::attachShader(const char* path, ShaderType type) const {
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