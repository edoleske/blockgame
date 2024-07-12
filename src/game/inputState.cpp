#include "inputState.h"

InputState* InputState::_instance = nullptr;

InputState::InputState() {
    InputState::_instance = this;

    for (auto const&[event, key] : InputEventKeyMap) {
        keyStateMap[key] = KeyState();
    }
}

InputState::~InputState() {
    if (_instance == this) {
        _instance = nullptr;
    }
}

KeyState InputState::getState(InputEvent event) const {
    return keyStateMap.at(InputEventKeyMap.at(event));
}

void InputState::updateKey(int key, bool state) {
    if (!keyStateMap.contains(key)) return;

    keyStateMap[key].previous = keyStateMap[key].current;
    keyStateMap[key].current = state;
}

void InputState::registerCallback(GLFWwindow* window) {
    glfwSetKeyCallback(window, InputState::callback);
}

void InputState::callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (_instance != nullptr) {
        _instance->updateKey(key, action != GLFW_RELEASE);
    }
}
