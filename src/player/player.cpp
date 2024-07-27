#include "player.h"

#include "../game/world.h"

Player::Player(float fov, float aspectRatio) : camera(fov, aspectRatio) {}

const Camera& Player::getCamera() const {
    return camera;
}

const vec3& Player::getSize() const {
    return size;
}

void Player::updateAspectRatio(int width, int height) {
    camera.setAspectRatio(width, height);
}

void Player::onMove(const vec3& velocity, const unique_ptr<World>& world) {
    vec3 adjustedVelocity = velocity * SPEED;
    vec3 position = camera.getPosition();

    auto front = camera.getFront();
    auto up = camera.getUp();

    vec3 movement = vec3(0.0f);
    movement += adjustedVelocity.x * glm::normalize(glm::cross(front, up));
    movement += adjustedVelocity.y * up;
    movement += adjustedVelocity.z * front;

    // Test for collisions
    vec3 cornerPosition = position - CENTER_OFFSET;
    ivec3 startIndex = glm::floor(cornerPosition);
    ivec3 endIndex = glm::floor(cornerPosition + size);

    if (testCollision(vec3(cornerPosition.x, cornerPosition.y + movement.y, cornerPosition.z), world)) {
        movement.y = 0;
    }
    if (testCollision(vec3(cornerPosition.x + movement.x, cornerPosition.y, cornerPosition.z), world)) {
        movement.x = 0;
    }
    if (testCollision(vec3(cornerPosition.x, cornerPosition.y, cornerPosition.z + movement.z), world)) {
        movement.z = 0;
    }

    camera.move(position + movement);
}

void Player::onRotate(float xOffset, float yOffset) {
    camera.rotate(xOffset * ROTATE_SENSITIVITY, yOffset * ROTATE_SENSITIVITY);
}

bool Player::testCollision(const vec3& position, const unique_ptr<World>& world) const {
    for (int x = floor(position.x); x < floor(position.x + size.x) + 1; x++) {
        for (int y = floor(position.y); y < floor(position.y + size.y) + 1; y++) {
            for (int z = floor(position.z); z < floor(position.z + size.z) + 1; z++) {
                auto block = world->getBlock(x, y, z);
                if (block.has_value() && block->isOpaque()) {
                    if (position.x < x + 1 && position.x + size.x > x &&
                        position.y < y + 1 && position.y + size.y > y &&
                        position.z < z + 1 && position.z + size.z > z) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}
