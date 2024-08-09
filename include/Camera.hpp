#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

using namespace glm;


class Camera {

public:
    int width;
    int height;
    float fov;
    float nearClip;
    float farClip;

    vec3 position;
    quat orientation;

    vec4 farPlane;
    vec4 leftPlane;
    vec4 rightPlane;
    vec4 upPlane;
    vec4 downPlane;
    mat4 vpMatrix;

    /**
     * @brief Create a new camera
     * @param width Window width (in pixels)
     * @param height Window height (in pixels)
     * @param fov Vertical FOV (in degrees)
     * @param nearClip Near clipping plane distance
     * @param farClip Far clipping plane distance
     * @param position Initial position of the camera
     * @param xOrientation Initial orientation around the x axis (in radians)
     * @param yOrientation Initial orientation around the y axis (in radians)
    **/
    Camera(int width, int height, float fov = 60, float nearClip = 0.1, float farClip = 1500, vec3 position = vec3(0, 0, 0), float xOrientation = 0, float yOrientation = 0);

    /**
     * @brief Update matrix and planes based on position and orientation (must be called after changing position or orientation)
    **/
    void update();

    /**
     * @brief Translate the camera
     * @param translation Translation vector (in camera space)
    **/
    void localTranslate(vec3 translation);

    /**
     * @brief Translate the camera
     * @param translation Translation vector (in world space)
    **/
    void globalTranslate(vec3 translation);

    /**
     * @brief Rotate the camera
     * @param xRotation Rotation around the x axis (in radians)
     * @param yRotation Rotation around the y axis (in radians)
    **/
    void rotate(float xRotation, float yRotation);

};


#endif // CAMERA_H