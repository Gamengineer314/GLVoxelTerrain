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
    window = glfwCreateWindow(width, height, "Voxel terrain", nullptr, nullptr);
    if (window == nullptr) throw runtime_error("glfwCreateWindow error");
    glfwMakeContextCurrent(window);
    if (!vsync) glfwSwapInterval(0);
    getCursorPosition(lastCursorX, lastCursorY);
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


bool Window::closed() {
    return glfwWindowShouldClose(window);
}


bool Window::keyPressed(int key) {
    return glfwGetKey(window, key) == GLFW_PRESS;
}


bool Window::mouseButtonPressed(int button) {
    return glfwGetMouseButton(window, button) == GLFW_PRESS;
}


void Window::getCursorPosition(double& x, double& y) {
    glfwGetCursorPos(window, &x, &y);
}


void Window::setCursorPosition(double x, double y) {
    glfwSetCursorPos(window, x, y);
}


void Window::getCursorMovement(double& dx, double& dy) {
    dx = cursorDx;
    dy = cursorDy;
    cursorDx = 0;
    cursorDy = 0;
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


void Window::dispose() {
    glfwDestroyWindow(window);
    glfwTerminate();
}