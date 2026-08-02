#ifndef BLOCKGAME_SETTINGS_H
#define BLOCKGAME_SETTINGS_H

#include "common.h"
#include "input.h"


// Singleton that persists user-selected settings
class Settings {
public:
    Settings();

    ~Settings();

    static Settings* getInstance();

    void load();

    void save();

    // Chunk radius to load and render
    int renderDistance = 12;

    static inline unordered_map<int, Input::Event> keymap = {
        {GLFW_MOUSE_BUTTON_LEFT,  Input::Event::MINE_BLOCK},
        {GLFW_MOUSE_BUTTON_RIGHT, Input::Event::PLACE_BLOCK},
        {GLFW_KEY_A,              Input::Event::MOVE_LEFT},
        {GLFW_KEY_D,              Input::Event::MOVE_RIGHT},
        {GLFW_KEY_W,              Input::Event::MOVE_FRONT},
        {GLFW_KEY_S,              Input::Event::MOVE_BACK},
        {GLFW_KEY_SPACE,          Input::Event::MOVE_UP},
        {GLFW_KEY_LEFT_SHIFT,     Input::Event::MOVE_DOWN},
        {GLFW_KEY_LEFT_CONTROL,   Input::Event::SPRINT},
        {GLFW_KEY_F,              Input::Event::TOGGLE_FLY},
        {GLFW_KEY_B,              Input::Event::TOGGLE_DEBUG},
        {GLFW_KEY_1,              Input::Event::ITEM_1},
        {GLFW_KEY_2,              Input::Event::ITEM_2},
        {GLFW_KEY_3,              Input::Event::ITEM_3},
        {GLFW_KEY_4,              Input::Event::ITEM_4},
        {GLFW_KEY_5,              Input::Event::ITEM_5},
        {GLFW_KEY_6,              Input::Event::ITEM_6},
        {GLFW_KEY_7,              Input::Event::ITEM_7},
        {GLFW_KEY_8,              Input::Event::ITEM_8},
        {GLFW_KEY_9,              Input::Event::ITEM_9},
        {GLFW_KEY_0,              Input::Event::ITEM_0},
    };
private:
    static Settings* _instance;
};


#endif //BLOCKGAME_SETTINGS_H
