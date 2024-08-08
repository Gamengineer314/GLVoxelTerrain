#include "GLObjects/Shader.hpp"

#include <iostream>
#include <fstream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GLObjects/Buffer.hpp"

using namespace std;
using namespace glm;

static GLuint usedProgram = 0;



char* readAllText(const char* path) {
    ifstream file = ifstream(path);
    file.seekg(0, ios::end);
    size_t size = file.tellg();
    char* text = new char[size + 1];
    file.seekg(0);
    file.read(text, size);
    text[size] = 0;
    return text;
}


void printCompileError(GLuint shader, const char* name) {
    GLint isCompiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE) {
        GLint length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        char* log = new char[length];
        glGetShaderInfoLog(shader, length, &length, log);
        cout << "Shader compile error when compiling " << name << " : \n" << log;
        delete[] log;
        return;
    }
}



Shader::Shader(const char* vertexPath, const char* fragmentPath) :
    program(glCreateProgram()) {

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    char* vertexCode = readAllText(vertexPath);
    glShaderSource(vertexShader, 1, &vertexCode, nullptr);
    delete[] vertexCode;
    glCompileShader(vertexShader);
    printCompileError(vertexShader, vertexPath);
    glAttachShader(program, vertexShader);
    glDeleteShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    char* fragmentCode = readAllText(fragmentPath);
    glShaderSource(fragmentShader, 1, &fragmentCode, nullptr);
    delete[] fragmentCode;
    glCompileShader(fragmentShader);
    printCompileError(fragmentShader, fragmentPath);
    glAttachShader(program, fragmentShader);
    glDeleteShader(fragmentShader);

    glLinkProgram(program);
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

void Shader::Uniform::setValue(float value) {
    shader.use();
    glUniform1f(location, value);
}