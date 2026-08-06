#include "player.h"

#include "game/input.h"
#include "game/block/blockDictionary.h"
#include "game/item/itemBlock.h"
#include "game/world/world.h"

Player::Player(float fov, float aspectRatio) : camera(fov, aspectRatio) {}

const Camera& Player::getCamera() const {
    return camera;
}

const Inventory& Player::getInventory() const {
    return inventory;
}

const vec3& Player::getSize() const {
    return size;
}

bool Player::isFlying() const {
    return flying;
}

void Player::update(float deltaTime, const unique_ptr<World>& world) {
    const auto input = Input::getInstance();

    auto fly = input->getState(Input::Event::TOGGLE_FLY);
    if (fly.current && !fly.previous) {
        flying = !flying;
    }

    vec3 movementVector = vec3(0.0f);

    if (input->getState(Input::Event::MOVE_FRONT).current) {
        movementVector.z += 1.0f;
    }
    if (input->getState(Input::Event::MOVE_BACK).current) {
        movementVector.z -= 1.0f;
    }
    if (input->getState(Input::Event::MOVE_LEFT).current) {
        movementVector.x -= 1.0f;
    }
    if (input->getState(Input::Event::MOVE_RIGHT).current) {
        movementVector.x += 1.0f;
    }
    if (input->getState(Input::Event::MOVE_UP).current) {
        if (flying) {
            movementVector.y += 1.0f;
        } else if (!input->getState(Input::Event::MOVE_UP).previous && jumpVelocity.y == 0.0f) {
            jumpVelocity.y = 0.95f;
        }
    }
    if (input->getState(Input::Event::MOVE_DOWN).current && flying) {
        movementVector.y -= 1.0f;
    }

    jumpVelocity.y = std::max(GRAVITY * deltaTime + jumpVelocity.y, GRAVITY * 5);
    auto currentVelocity = flying ? vec3(0.0f) : jumpVelocity;

    if (glm::length(movementVector) != 0) {
        currentVelocity += glm::normalize(movementVector);
    }

    onMove(currentVelocity * deltaTime, world);

    auto cursorOffset = input->getCursorOffset();
    onRotate(cursorOffset.x, cursorOffset.y);

    // Update Inventory
    if (input->isPressed(Input::Event::SCROLL_UP)) {
        inventory.setSelected(inventory.getSelected() + 1);
    }
    if (input->isPressed(Input::Event::SCROLL_DOWN)) {
        inventory.setSelected(inventory.getSelected() - 1);
    }

    for (auto& event : {
             Input::Event::ITEM_1, Input::Event::ITEM_2, Input::Event::ITEM_3, Input::Event::ITEM_4,
             Input::Event::ITEM_5,
             Input::Event::ITEM_6, Input::Event::ITEM_7, Input::Event::ITEM_8, Input::Event::ITEM_9,
             Input::Event::ITEM_0
         }) {
        if (input->isPressed(event)) {
            inventory.setSelected(static_cast<int>(event) - static_cast<int>(Input::Event::ITEM_1));
        }
    }

    if (input->isPressed(Input::Event::MINE_BLOCK)) {
        const auto block = world->mineBlock(camera.getPosition(), camera.getFront());
        if (block.has_value()) {
            auto stack = ItemStack(make_unique<ItemBlock>(block.value()), 1);
            inventory.insert(stack);
        }
    }

    if (input->isPressed(Input::Event::PLACE_BLOCK)) {
        auto held = inventory.getItemStack(inventory.getSelected());
        if (held->item->getName() != "0" && held->amount > 0) {
            const auto itemBlock = dynamic_cast<ItemBlock*>(held->item.get());
            if (itemBlock != nullptr) {
                const BlockType blockType = itemBlock->getBlockType();
                auto result = world->placeBlock(blockType, camera.getPosition(), camera.getFront());

                if (result) {
                    inventory.pop(inventory.getSelected());
                }
            }
        }
    }
}

void Player::updateAspectRatio(const float aspectRatio) {
    camera.setAspectRatio(aspectRatio);
}

void Player::onMove(const vec3& velocity, const unique_ptr<World>& world) {
    vec3 adjustedVelocity = velocity * SPEED;
    vec3 position = camera.getPosition();

    auto front = camera.getFront();
    auto up = camera.getUp();

    vec3 movement = vec3(0.0f);
    movement += adjustedVelocity.x * glm::normalize(glm::cross(front, up));
    movement += adjustedVelocity.y * up;
    movement += adjustedVelocity.z * glm::normalize(vec3(front.x, 0.0f, front.z));

    // Test for collisions
    vec3 cornerPosition = position - CENTER_OFFSET;

    if (testCollision(vec3(cornerPosition.x, cornerPosition.y + movement.y, cornerPosition.z), cornerPosition, world)) {
        movement.y = 0;
        jumpVelocity.y = 0;
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
                if (block.has_value() && BlockDictionary::getInstance()->get(block.value()).opaque) {
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
