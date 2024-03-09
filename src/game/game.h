#ifndef BLOCKGAME_GAME_H
#define BLOCKGAME_GAME_H


#include "common.h"
#include "gl/window.h"
#include "gl/vertexBuffer.h"
#include "gl/vertexArray.h"
#include "camera.h"
#include "gl/shader.h"
#include "world.h"
#include "gl/texture.h"

class Game : public Window {
public:
    Game(int width, int height);

    void loop() override;
private:
    double lastMouseX = 0.0, lastMouseY = 0.0;
    unique_ptr<Camera> camera;
    unique_ptr<Shader> shader;
    unique_ptr<World> world;

    // Temporary block texture
    unique_ptr<Texture> texture;

    void handleInput();
    void handleMouse();

    VertexArray vao;
    VertexBuffer vbo;
};


#endif //BLOCKGAME_GAME_H
