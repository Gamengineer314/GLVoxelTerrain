#ifndef LOG_H
#define LOG_H

#include <glad/glad.h>

/**
 * @brief OpenGL message callback
**/
void GLAPIENTRY glCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

#endif