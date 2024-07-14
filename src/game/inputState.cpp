#include "inputState.h"

InputState* InputState::_instance = nullptr;

InputState::InputState() : currentCursor(0.0, 0.0), previousCursor(0.0, 0.0) {
    InputState::_instance = this;

    for (auto const& [key, event]: InputEventMap) {
        inputMap[event] = ButtonState();
    }
}

InputState::~InputState() {
    if (_instance == this) {
        _instance = nullptr;
    }
}

ButtonState InputState::getState(InputEvent event) const {
    return inputMap.at(event);
}

vec2 InputState::getCursorOffset() {
    auto offset = vec2(static_cast<float>(currentCursor.x - previousCursor.x),
                       static_cast<float>(previousCursor.y - currentCursor.y));
    previousCursor = currentCursor;

    return offset;
}

void InputState::updateKey(int key, bool state) {
    if (!InputEventMap.contains(key)) return;

    auto event = InputEventMap[key];
    inputMap[event].previous = inputMap[event].current;
    inputMap[event].current = state;
}

void InputState::updateCursor(double x, double y) {
    currentCursor = glm::vec<2, double>(x, y);
}

void InputState::registerCallbacks(GLFWwindow* window) {
    glfwSetKeyCallback(window, InputState::keyCallback);
    glfwSetMouseButtonCallback(window, InputState::mouseButtonCallback);
    glfwSetCursorPosCallback(window, InputState::cursorPositionCallback);
}

void InputState::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (_instance != nullptr) {
        _instance->updateKey(key, action != GLFW_RELEASE);
    }
}

void InputState::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (_instance != nullptr) {
        // Mouse button codes (0-8) are compatible with key codes (32-348)
        _instance->updateKey(button, action == GLFW_PRESS);
    }
}

void InputState::cursorPositionCallback(GLFWwindow* window, double x, double y) {
    if (_instance != nullptr) {
        _instance->updateCursor(x, y);
    }
}
