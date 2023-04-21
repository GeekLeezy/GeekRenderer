#include "camera.h"

void Camera::updateCameraVectors()
{
    // 根据欧拉角计算dir
    vec3 direction;
    direction.x = cos(radians(yaw)) * cos(radians(pitch));
    direction.y = sin(radians(pitch));
    direction.z = sin(radians(yaw)) * cos(radians(pitch));
    dir = normalize(direction);
    // 重新计算right、up向量
    // 归一化防止运动速度的不一致
    right = normalize(cross(dir, worldUp));
    up = normalize(cross(right, dir));
}

mat4 Camera::getViewMatrix()
{
    mat4 viewMat(1.0f);
    vec3 d = normalize(dir);
    vec3 r = normalize(cross(d, up));
    vec3 u = cross(r, d);

    viewMat[0][0] = r.x;
    viewMat[1][0] = r.y;
    viewMat[2][0] = r.z;
    viewMat[0][1] = u.x;
    viewMat[1][1] = u.y;
    viewMat[2][1] = u.z;
    viewMat[0][2] = -d.x;
    viewMat[1][2] = -d.y;
    viewMat[2][2] = -d.z;
    viewMat[3][0] = -dot(r, position);
    viewMat[3][1] = -dot(u, position);
    viewMat[3][2] = dot(d, position);

    return viewMat;
}

mat4 Camera::getProjectMatrix()
{
    mat4 projectMat(0.0f);

    float tanHalfFovY = tan(fov / 2.0f);

    projectMat[0][0] = 1.0f / (aspect * tanHalfFovY);
    projectMat[1][1] = 1.0f / tanHalfFovY;
    projectMat[2][2] = (n + f) / (n - f);
    projectMat[2][3] = -1.0f;
    projectMat[3][2] = 2.0f * n * f / (n - f);

    return projectMat;
}

void Camera::updateFov(float fov) {
    fov = glm::radians(fov);
}

void Camera::updateAspect(int w, int h) {
    aspect = (float)w / h;
}

void Camera::moveForward(float distance) {
    position += dir * distance;
}

void Camera::moveRight(float distance) {
    position += right * distance;
}

void Camera::moveUp(float distance) {
    position += up * distance;
}

void Camera::rotatePitch(float angle) {
    pitch += angle;
    if (pitch > 89.0)
        pitch = 89.0;
    if (pitch < -89.0)
        pitch = -89.0;
    updateCameraVectors();
}

void Camera::rotateYaw(float angle) {
    yaw += angle;
    updateCameraVectors();
}

