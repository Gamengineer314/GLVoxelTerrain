#include "Camera.hpp"

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

using namespace std;

using namespace glm;


// Reverse x and z axes to convert from view (x+ = left, z+ = forward) to screen view (x+ = right, z+ = backward)
#define VIEW_TO_SCREENVIEW mat4(-1,0,0,0, 0,1,0,0, 0,0,-1,0, 0,0,0,1)


Camera::Camera(int width, int height, float fov, float nearClip, float farClip, vec3 position, float xOrientation, float yOrientation) : 
    width(width), 
    height(height), 
    fov(radians(fov)), 
    nearClip(nearClip), 
    farClip(farClip), 
    position(position),
    orientation(quat(cos(yOrientation / 2), 0, sin(yOrientation / 2), 0) * quat(cos(xOrientation / 2), sin(xOrientation / 2), 0, 0)) {

    update();
}


void Camera::update() {
    mat4 view = VIEW_TO_SCREENVIEW * translate(mat4_cast(quat(orientation.w, -orientation.x, -orientation.y, -orientation.z)), -position);
    mat4 projection = perspective(fov, (float)width / height, 0.1f, 1000.0f);
    vpMatrix = projection * view;
}


void Camera::localTranslate(vec3 translation) {
    position += orientation * translation;
}


void Camera::globalTranslate(vec3 translation) {
    position += translation;
}


void Camera::rotate(float xRotation, float yRotation) {
    orientation = quat(cos(yRotation / 2), 0, sin(yRotation / 2), 0) * orientation * quat(cos(xRotation / 2), sin(xRotation / 2), 0, 0);
}