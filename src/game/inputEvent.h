#ifndef BLOCKGAME_INPUTEVENT_H
#define BLOCKGAME_INPUTEVENT_H

#include "common.h"

enum class InputEvent {
    MOVE_LEFT, MOVE_RIGHT,
    MOVE_FRONT, MOVE_BACK,
    MOVE_UP, MOVE_DOWN,
    SPRINT, TOGGLE_FLY,
    MINE_BLOCK, PLACE_BLOCK
};

static unordered_map<InputEvent, int> InputEventKeyMap = {
        {InputEvent::MOVE_LEFT, GLFW_KEY_A},
        {InputEvent::MOVE_RIGHT, GLFW_KEY_D},
        {InputEvent::MOVE_FRONT, GLFW_KEY_W},
        {InputEvent::MOVE_BACK, GLFW_KEY_S},
        {InputEvent::MOVE_UP, GLFW_KEY_SPACE},
        {InputEvent::MOVE_DOWN, GLFW_KEY_LEFT_SHIFT},
        {InputEvent::SPRINT, GLFW_KEY_LEFT_CONTROL},
        {InputEvent::TOGGLE_FLY, GLFW_KEY_F},
};

#endif //BLOCKGAME_INPUTEVENT_H
