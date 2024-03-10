#ifndef BLOCKGAME_CAMERA_H
#define BLOCKGAME_CAMERA_H


#include "common.h"

class Camera {
public:
    Camera(float fov, float aspectRatio);

    const mat4 &getProjection() const;
    const mat4 &getView() const;

    void setAspectRatio(int width, int height);

    void move(const vec3 &normalizedVelocity, float deltaTime);
    void rotate(float xOffset, float yOffset);

    void recalculateProjectionMatrix();
    void recalculateViewMatrix();
private:
    float fov, aspectRatio;
    float pitch = 0.0f, yaw = 0.0f;

    vec3 position = vec3(0.0f, 66.0f, 0.0f);
    vec3 front = vec3(0.0f, 0.0f, -1.0f);
    vec3 up = vec3(0.0f, 1.0f, 0.0f);

    mat4 projection = mat4(1.0f);
    mat4 view = mat4(1.0f);

    const float SPEED = 8.0f;
    const float ROTATE_SENSITIVITY = 0.1f;
};


#endif //BLOCKGAME_CAMERA_H
