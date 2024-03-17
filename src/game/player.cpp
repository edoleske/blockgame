#include "player.h"

Player::Player(float fov, float aspectRatio): camera(fov, aspectRatio) {}

const Camera& Player::getCamera() const {
    return camera;
}

void Player::updateAspectRatio(int width, int height) {
    camera.setAspectRatio(width, height);
}

void Player::onMove(const vec3& velocity) {
    vec3 adjustedVelocity = velocity * SPEED;
    vec3 newPosition = camera.getPosition();

    auto front = camera.getFront();
    auto up = camera.getUp();

    newPosition += adjustedVelocity.x * glm::normalize(glm::cross(front, up));
    newPosition += adjustedVelocity.y * up;
    newPosition += adjustedVelocity.z * front;

    // Test new position for collision, only move if position is valid

    camera.move(newPosition);
}

void Player::onRotate(float xOffset, float yOffset) {
    camera.rotate(xOffset * ROTATE_SENSITIVITY, yOffset * ROTATE_SENSITIVITY);
}
