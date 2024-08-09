#include "GLObjects/Log.hpp"

#include <iostream>
#include <unordered_map>
#include <glad/glad.h>

using namespace std;


unordered_map<GLenum, string> callbackSources = {
    { GL_DEBUG_SOURCE_API, "API" },
    { GL_DEBUG_SOURCE_WINDOW_SYSTEM, "Window system" },
    { GL_DEBUG_SOURCE_SHADER_COMPILER, "Shader compiler" },
    { GL_DEBUG_SOURCE_THIRD_PARTY, "Third party" },
    { GL_DEBUG_SOURCE_APPLICATION, "Application" },
    { GL_DEBUG_SOURCE_OTHER, "Other" }
};

unordered_map<GLenum, string> callbackTypes = {
    { GL_DEBUG_TYPE_ERROR, "Error" },
    { GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR, "Deprecated behavior" },
    { GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR, "Undefined behavior" },
    { GL_DEBUG_TYPE_PORTABILITY, "Portability" },
    { GL_DEBUG_TYPE_PERFORMANCE, "Performance" },
    { GL_DEBUG_TYPE_MARKER, "Marker" },
    { GL_DEBUG_TYPE_PUSH_GROUP, "Push group" },
    { GL_DEBUG_TYPE_POP_GROUP, "Pop group" },
    { GL_DEBUG_TYPE_OTHER, "Other" }
};

unordered_map<GLenum, string> callbackSeverities = {
    { GL_DEBUG_SEVERITY_HIGH, "High" },
    { GL_DEBUG_SEVERITY_MEDIUM, "Medium" },
    { GL_DEBUG_SEVERITY_LOW, "Low" },
    { GL_DEBUG_SEVERITY_NOTIFICATION, "Notification" }
};

void glCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    if (severity != GL_DEBUG_SEVERITY_NOTIFICATION) {
        cerr << "OpenGL callback. Source: " << callbackSources[source] << ", Type: " << callbackTypes[type] << ", Severity: " << callbackSeverities[severity] << ", ID: " << id << endl <<
                "Message: " << message << endl << endl;
    }
}