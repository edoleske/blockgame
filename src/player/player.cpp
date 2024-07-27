#include "player.h"

#include "../game/world.h"

Player::Player(float fov, float aspectRatio) : camera(fov, aspectRatio) {}

const Camera& Player::getCamera() const {
    return camera;
}

const vec3& Player::getSize() const {
    return size;
}

bool Player::isFlying() const {
    return flying;
}

void Player::update(float deltaTime, InputState& input, const unique_ptr<World>& world) {
    auto fly = input.getState(InputEvent::TOGGLE_FLY);
    if (fly.current && !fly.previous) {
        flying = !flying;
    }

    vec3 velocity = vec3(0.0f);

    if (input.getState(InputEvent::MOVE_FRONT).current) {
        velocity.z += 1.0f;
    }
    if (input.getState(InputEvent::MOVE_BACK).current) {
        velocity.z -= 1.0f;
    }
    if (input.getState(InputEvent::MOVE_LEFT).current) {
        velocity.x -= 1.0f;
    }
    if (input.getState(InputEvent::MOVE_RIGHT).current) {
        velocity.x += 1.0f;
    }
    if (input.getState(InputEvent::MOVE_UP).current) {
        if (flying) {
        velocity.y += 1.0f;
        } else if (!input.getState(InputEvent::MOVE_UP).previous) {
            // Todo: Add jump here
        }
    }
    if (input.getState(InputEvent::MOVE_DOWN).current && flying) {
        velocity.y -= 1.0f;
    }

    if (glm::length(velocity) != 0) {
        onMove(glm::normalize(velocity) * deltaTime, world);
    }

    // Apply gravity
    if (!flying) {
        vec3 cornerPosition = camera.getPosition() - CENTER_OFFSET;
        vec3 gravity = vec3(0.0f, -10.0f, 0.0f) * deltaTime;
        if (!testCollision(cornerPosition + gravity, cornerPosition, world)) {
            camera.move(camera.getPosition() + gravity);
        }
    }

    auto cursorOffset = input.getCursorOffset();
    onRotate(cursorOffset.x, cursorOffset.y);

    auto mine = input.getState(InputEvent::MINE_BLOCK);
    if (mine.current && !mine.previous) {
        world->mineBlock(camera.getPosition(), camera.getFront());
    }

    auto place = input.getState(InputEvent::PLACE_BLOCK);
    if (place.current && !place.previous) {
        world->placeBlock(camera.getPosition(), camera.getFront());
    }

    // Gravity
    if (!flying) {
        vec3 cornerPosition = camera.getPosition() - CENTER_OFFSET;
        vec3 gravity = vec3(0.0f, -10.0f, 0.0f) * deltaTime;
        if (!testCollision(cornerPosition + gravity, cornerPosition, world)) {
            camera.move(camera.getPosition() + gravity);
        }
    }
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

    if (testCollision(vec3(cornerPosition.x, cornerPosition.y + movement.y, cornerPosition.z), cornerPosition, world)) {
        movement.y = 0;
    }
    if (testCollision(vec3(cornerPosition.x + movement.x, cornerPosition.y, cornerPosition.z), cornerPosition, world)) {
        movement.x = 0;
    }
    if (testCollision(vec3(cornerPosition.x, cornerPosition.y, cornerPosition.z + movement.z), cornerPosition, world)) {
        movement.z = 0;
    }

    camera.move(position + movement);
}

void Player::onRotate(float xOffset, float yOffset) {
    camera.rotate(xOffset * ROTATE_SENSITIVITY, yOffset * ROTATE_SENSITIVITY);
}

bool Player::testCollision(const vec3& position, const vec3& oldPosition, const unique_ptr<World>& world) const {
    for (int x = floor(position.x); x < floor(position.x + size.x) + 1; x++) {
        for (int y = floor(position.y); y < floor(position.y + size.y) + 1; y++) {
            for (int z = floor(position.z); z < floor(position.z + size.z) + 1; z++) {
                auto block = world->getBlock(x, y, z);
                if (block.has_value() && block->isOpaque()) {
                    // Ignore blocks already in bounding box (prevents player from getting stuck)
                    if (oldPosition.x < x + 1 && oldPosition.x + size.x > x &&
                        oldPosition.y < y + 1 && oldPosition.y + size.y > y &&
                        oldPosition.z < z + 1 && oldPosition.z + size.z > z) {
                        continue;
                    }

                    // Check AABB Collision
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
