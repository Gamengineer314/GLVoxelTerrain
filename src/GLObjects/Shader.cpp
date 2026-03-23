#include "GLObjects/Shader.hpp"

#include <fstream>
#include <sstream>

using namespace std;

namespace gl {

void Shader::attachShader(char const* path, ShaderType type) const {
    // Read file
    ifstream file = ifstream(path);
    stringstream stream;
    stream << file.rdbuf();
    string str = stream.str();
    char const* shaderCode = str.c_str();
    file.close();

    // Compile and attach shader
    GLuint shader = glCreateShader((GLenum)type);
    glShaderSource(shader, 1, &shaderCode, nullptr);
    glCompileShader(shader);
    glAttachShader(program, shader);
    glDeleteShader(shader);
}

}