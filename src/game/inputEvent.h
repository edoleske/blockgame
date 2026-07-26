#ifndef BLOCKGAME_INPUTEVENT_H
#define BLOCKGAME_INPUTEVENT_H

#include "common.h"

enum class InputEvent {
    MOVE_LEFT, MOVE_RIGHT, MOVE_FRONT, MOVE_BACK,
    MOVE_UP, MOVE_DOWN, SPRINT, TOGGLE_FLY,
    MINE_BLOCK, PLACE_BLOCK, SCROLL_UP, SCROLL_DOWN,
    TOGGLE_DEBUG,
    ITEM_1, ITEM_2, ITEM_3, ITEM_4, ITEM_5, ITEM_6, ITEM_7, ITEM_8, ITEM_9, ITEM_0
};

static unordered_map<int, InputEvent> InputEventMap = {
        {GLFW_MOUSE_BUTTON_LEFT,  InputEvent::MINE_BLOCK},
        {GLFW_MOUSE_BUTTON_RIGHT, InputEvent::PLACE_BLOCK},
        {GLFW_KEY_A,              InputEvent::MOVE_LEFT},
        {GLFW_KEY_D,              InputEvent::MOVE_RIGHT},
        {GLFW_KEY_W,              InputEvent::MOVE_FRONT},
        {GLFW_KEY_S,              InputEvent::MOVE_BACK},
        {GLFW_KEY_SPACE,          InputEvent::MOVE_UP},
        {GLFW_KEY_LEFT_SHIFT,     InputEvent::MOVE_DOWN},
        {GLFW_KEY_LEFT_CONTROL,   InputEvent::SPRINT},
        {GLFW_KEY_F,              InputEvent::TOGGLE_FLY},
        {GLFW_KEY_B,              InputEvent::TOGGLE_DEBUG},
        {GLFW_KEY_1,              InputEvent::ITEM_1},
        {GLFW_KEY_2,              InputEvent::ITEM_2},
        {GLFW_KEY_3,              InputEvent::ITEM_3},
        {GLFW_KEY_4,              InputEvent::ITEM_4},
        {GLFW_KEY_5,              InputEvent::ITEM_5},
        {GLFW_KEY_6,              InputEvent::ITEM_6},
        {GLFW_KEY_7,              InputEvent::ITEM_7},
        {GLFW_KEY_8,              InputEvent::ITEM_8},
        {GLFW_KEY_9,              InputEvent::ITEM_9},
        {GLFW_KEY_0,              InputEvent::ITEM_0},
};

#endif //BLOCKGAME_INPUTEVENT_H
