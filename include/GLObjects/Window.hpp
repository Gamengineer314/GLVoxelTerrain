#ifndef WINDOW_H
#define WINDOW_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>


class Window {

public:
    int width;
    int height;

    /**
     * @brief Create a GLFW window
     * @param width Window width (in pixels)
     * @param height Window height (in pixels)
     * @param title Window title
     * @param vsync Enable VSync
    **/
    Window(int width, int height, const char* title, bool vsync = false);

    /**
     * @brief Window update (must be called each frame)
    **/
    void update();

    /**
     * @brief Check if the window was closed
    **/
    bool closed();

    /**
     * @brief Check if a given key was pressed
     * @param key Key code of the key to check
    **/
    bool keyPressed(int key);

    /**
     * @brief Check if a given mouse button was pressed
     * @param button Button code of the button to check
    **/
    bool mouseButtonPressed(int button);

    /**
     * @brief Get the cursor position
     * @param x x position (output)
     * @param y y position (output)
    **/
    void getCursorPosition(double& x, double& y);

    /**
     * @brief Set the cursor position
     * @param x x position
     * @param y y position
    **/
    void setCursorPosition(double x, double y);

    /**
     * @brief Get the movement of the cursor since the last call to this function
     * @param dx x movement (output)
     * @param dy y movement (output)
    **/
    void getCursorMovement(double& dx, double& dy);

    /**
     * @brief Lock and hide the cursor
    **/
    void lockCursor();

    /**
     * @brief Unlock and show the cursor
    **/
    void unlockCursor();

    /**
     * @brief Delete the window
    **/
    void dispose();

private:
    GLFWwindow* window;
    double lastCursorX;
    double lastCursorY;
    double cursorDx = 0;
    double cursorDy = 0;
    bool locked = false;

};


#endif // WINDOW_H