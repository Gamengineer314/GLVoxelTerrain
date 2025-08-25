#include "GLObjects/Window.hpp"

#include <stdexcept>
#include <GLFW/glfw3.h>
#include <glad/glad.h>

using namespace std;


Window::Window(int width, int height, const char* title, bool vsync) :
    width(width),
    height(height) {

    // Init GLFW
    if (glfwInit() == GLFW_FALSE) throw runtime_error("glfwInit error");
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    // Create window
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (window == nullptr) throw runtime_error("glfwCreateWindow error");
    glfwMakeContextCurrent(window);
    if (!vsync) glfwSwapInterval(0);
    getCursorPosition(lastCursorX, lastCursorY);
}


Window::~Window() {
    glfwDestroyWindow(window);
    glfwTerminate();
}


void Window::update() {
    glfwPollEvents();
    glfwSwapBuffers(window);

    // Cursor movement and lock
    double cursorX, cursorY;
    getCursorPosition(cursorX, cursorY);
    cursorDx += cursorX - lastCursorX;
    cursorDy += cursorY - lastCursorY;

    if (locked) {
        setCursorPosition(width / 2, height / 2);
        if (keyPressed(GLFW_KEY_ESCAPE)) unlockCursor();
    }
    else {
        lastCursorX = cursorX;
        lastCursorY = cursorY;
        if (mouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) lockCursor();
    }
}


void Window::lockCursor() {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    setCursorPosition(width / 2, height / 2);
    lastCursorX = width / 2;
    lastCursorY = height / 2;
    locked = true;
}


void Window::unlockCursor() {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    locked = false;
}