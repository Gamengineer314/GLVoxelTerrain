#include "Camera.hpp"

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

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
    mat4 projection = perspective(fov, (float)width / height, nearClip, farClip);
    vpMatrix = projection * view;
    farPlane = vec4(vpMatrix[0][3] - vpMatrix[0][2], vpMatrix[1][3] - vpMatrix[1][2], vpMatrix[2][3] - vpMatrix[2][2], vpMatrix[3][3] - vpMatrix[3][2]);
    farPlane = farPlane / length(vec3(farPlane));
    leftPlane = vec4(vpMatrix[0][3] + vpMatrix[0][0], vpMatrix[1][3] + vpMatrix[1][0], vpMatrix[2][3] + vpMatrix[2][0], vpMatrix[3][3] + vpMatrix[3][0]);
    leftPlane = leftPlane / length(vec3(leftPlane));
    rightPlane = vec4(vpMatrix[0][3] - vpMatrix[0][0], vpMatrix[1][3] - vpMatrix[1][0], vpMatrix[2][3] - vpMatrix[2][0], vpMatrix[3][3] - vpMatrix[3][0]);
    rightPlane = rightPlane / length(vec3(rightPlane));
    upPlane = vec4(vpMatrix[0][3] - vpMatrix[0][1], vpMatrix[1][3] - vpMatrix[1][1], vpMatrix[2][3] - vpMatrix[2][1], vpMatrix[3][3] - vpMatrix[3][1]);
    upPlane = upPlane / length(vec3(upPlane));
    downPlane = vec4(vpMatrix[0][3] + vpMatrix[0][1], vpMatrix[1][3] + vpMatrix[1][1], vpMatrix[2][3] + vpMatrix[2][1], vpMatrix[3][3] + vpMatrix[3][1]);
    downPlane = downPlane / length(vec3(downPlane));
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