#include "camera.h"

Camera::Camera(float fov, float aspectRatio): fov(fov), aspectRatio(aspectRatio) {
    projection = glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 1000.0f);
    view = glm::lookAt(position, position + front, up);
}

const vec3& Camera::getPosition() const {
    return position;
}

const vec3& Camera::getFront() const {
    return front;
}

const vec3& Camera::getUp() const {
    return up;
}

const mat4 &Camera::getProjection() const {
    return projection;
}

const mat4 &Camera::getView() const {
    return view;
}

void Camera::setAspectRatio(int width, int height) {
    auto newAspectRatio = static_cast<float>(width) / static_cast<float>(height);
    if (newAspectRatio != aspectRatio) {
        aspectRatio = newAspectRatio;
        recalculateProjectionMatrix();
    }
}

void Camera::move(const vec3 &newPosition) {
    position = newPosition;
    recalculateViewMatrix();
}

void Camera::rotate(float xOffset, float yOffset) {
    yaw += xOffset;
    yaw = glm::mod(yaw, 360.0f);

    pitch += yOffset;
    pitch = glm::clamp(pitch, -89.0f, 89.0f);

    vec3 direction(
            cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
            sin(glm::radians(pitch)),
            sin(glm::radians(yaw)) * cos(glm::radians(pitch))
    );
    front = glm::normalize(direction);

    recalculateViewMatrix();
}

void Camera::recalculateProjectionMatrix() {
    projection = glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 100.0f);
}

void Camera::recalculateViewMatrix() {
    view = glm::lookAt(position, position + front, up);
}
