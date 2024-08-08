#include "CameraController.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

using namespace glm;


CameraController::CameraController(Window& window, Camera& camera, int width, int height, float sensitivityX, float sensitivityY, float speed) : 
    sensitivityX(sensitivityX), 
    sensitivityY(sensitivityY), 
    speed(speed),
    window(window), 
    camera(camera) {
}


void CameraController::update(float deltaTime) {
    // Position
    float xMove = 0;
    float zMove = 0;
    if (window.keyPressed(GLFW_KEY_A)) xMove += speed * deltaTime;
    if (window.keyPressed(GLFW_KEY_D)) xMove -= speed * deltaTime;
    if (window.keyPressed(GLFW_KEY_W)) zMove += speed * deltaTime;
    if (window.keyPressed(GLFW_KEY_S)) zMove -= speed * deltaTime;
    camera.localTranslate(vec3(xMove, 0, zMove));

    // Rotation
    double dx, dy;
    window.getCursorMovement(dx, dy);
    float xRotation = dy * sensitivityY;
    float yRotation = -dx * sensitivityX;
    camera.rotate(xRotation, yRotation);

    camera.update();
}