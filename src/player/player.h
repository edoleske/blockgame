#ifndef BLOCKGAME_PLAYER_H
#define BLOCKGAME_PLAYER_H


#include "common.h"
#include "camera.h"
#include "../game/inputState.h"

class World;

class Player {
public:
    Player(float fov, float aspectRatio);

    const Camera& getCamera() const;

    const vec3& getSize() const;

    bool isFlying() const;

    void update(float deltaTime, InputState& input, const unique_ptr<World>& world);

    void updateAspectRatio(int width, int height);

    void onMove(const vec3 &velocity, const unique_ptr<World>& world);

    void onRotate(float xOffset, float yOffset);

private:
    Camera camera;

    vec3 size = vec3(0.6f, 1.8f, 0.6f);
    const vec3 CENTER_OFFSET = vec3(0.5f, 0.8f, 0.5f) * size;

    bool flying = false;
    vec3 jumpVelocity = vec3(0.0f);

    const float SPEED = 10.0f;
    const float ROTATE_SENSITIVITY = 0.1f;
    const float GRAVITY = -2.0f;

    bool testCollision(const vec3& position, const vec3& oldPosition, const unique_ptr<World>& world) const;
};


#endif //BLOCKGAME_PLAYER_H
