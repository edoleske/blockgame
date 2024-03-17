#ifndef BLOCKGAME_PLAYER_H
#define BLOCKGAME_PLAYER_H


#include "common.h"
#include "camera.h"

class Player {
public:
    Player(float fov, float aspectRatio);

    const Camera& getCamera() const;

    void updateAspectRatio(int width, int height);

    void onMove(const vec3 &velocity);

    void onRotate(float xOffset, float yOffset);

private:
    Camera camera;

    const float SPEED = 8.0f;
    const float ROTATE_SENSITIVITY = 0.1f;
};


#endif //BLOCKGAME_PLAYER_H
