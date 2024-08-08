#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "GLObjects/Window.hpp"
#include "Camera.hpp"

using namespace glm;


class CameraController {

public:
    float sensitivityX;
    float sensitivityY;
    float speed;
    
    /**
     * @brief Create a new camera controller
     * @param window GLFW window
     * @param camera Camera to control
     * @param width Window width
     * @param height Window height
     * @param sensitivityX Mouse sensitivity on the x axis
     * @param sensitivityY Mouse sensitivity on the y axis
     * @param speed Camera speed
    **/
    CameraController(Window& window, Camera& camera, int width, int height, float sensitivityX = 0.003, float sensitivityY = 0.003, float speed = 200);

    /**
     * @brief CameraController update (must be called each frame)
     * @param deltaTime Last frame duration (in seconds)
    **/
    void update(float deltaTime);

private:
    Window& window;
    Camera& camera;

};


#endif