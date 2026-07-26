#ifndef BLOCKGAME_GAME_H
#define BLOCKGAME_GAME_H


#include "common.h"
#include "gl/window.h"
#include "gl/vertexBuffer.h"
#include "gl/vertexArray.h"
#include "gl/shader.h"
#include "world.h"
#include "player/player.h"
#include "inputState.h"
#include "settings.h"
#include "ui/uiRenderer.h"

class Game : public Window {
public:
    Game(int width, int height);

    void loop() override;

    void updateWindowSize(int w, int h) override;
private:
    InputState input;
    Settings settings;
    Player player;

    unique_ptr<World> world;
    unique_ptr<UIRenderer> uiRenderer;

    VertexArray vao;
    VertexBuffer vbo;

    void update();
};


#endif //BLOCKGAME_GAME_H
