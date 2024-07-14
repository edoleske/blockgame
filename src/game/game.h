#ifndef BLOCKGAME_GAME_H
#define BLOCKGAME_GAME_H


#include "common.h"
#include "gl/window.h"
#include "gl/vertexBuffer.h"
#include "gl/vertexArray.h"
#include "gl/shader.h"
#include "gl/texture.h"
#include "world.h"
#include "player/player.h"
#include "inputState.h"

class Game : public Window {
public:
    Game(int width, int height);

    void loop() override;

private:
    Player player;
    unique_ptr<Shader> shader;
    unique_ptr<World> world;
    InputState input;

    void handleInput();

    VertexArray vao;
    VertexBuffer vbo;
};


#endif //BLOCKGAME_GAME_H
