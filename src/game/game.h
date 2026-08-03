#ifndef BLOCKGAME_GAME_H
#define BLOCKGAME_GAME_H


#include "common.h"
#include "gl/window.h"
#include "gl/vertexBuffer.h"
#include "gl/vertexArray.h"
#include "gl/textureArray.h"
#include "world/world.h"
#include "player/player.h"
#include "input.h"
#include "settings.h"
#include "block/blockDictionary.h"
#include "ui/uiRenderer.h"

class Game : public Window {
public:
    Game(int width, int height);

    void loop() override;

    void updateWindowSize(int w, int h) override;

private:
    Input input;
    Settings settings;
    Player player;
    BlockDictionary blockDictionary;
    TextureArray textureArray;

    unique_ptr<World> world;
    unique_ptr<UIRenderer> uiRenderer;

    VertexArray vao;
    VertexBuffer vbo;

    void update();

    void initializeBlocks();

    void initializeTextureArray(const unordered_map<string, uint16_t>& textureLayerMap);
};


#endif //BLOCKGAME_GAME_H
