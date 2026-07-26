#include "inputState.h"

InputState* InputState::_instance = nullptr;

InputState::InputState() : currentCursor(0.0, 0.0), previousCursor(0.0, 0.0) {
    _instance = this;

    for (auto const& [key, event]: InputEventMap) {
        inputMap[event] = ButtonState();
    }

    // Scroll inputs are not mapped to a GLFW button, so we track it more manually
    inputMap[InputEvent::SCROLL_UP] = ButtonState();
    inputMap[InputEvent::SCROLL_DOWN] = ButtonState();
}

InputState::~InputState() {
    if (_instance == this) {
        _instance = nullptr;
    }
}

InputState* InputState::getInstance() {
    return _instance;
}

bool InputState::isPressed(const InputEvent event) const {
    auto [current, previous] = inputMap.at(event);
    return current && !previous;
}

bool InputState::isReleased(const InputEvent event) const {
    auto [current, previous] = inputMap.at(event);
    return !current && previous;
}

ButtonState InputState::getState(const InputEvent event) const {
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
    updateInputMap(event, state);
}

void InputState::updateCursor(double x, double y) {
    currentCursor = glm::vec<2, double>(x, y);
}

void InputState::updateScroll(double x, double y) {
    if (y > 0) {
        updateInputMap(InputEvent::SCROLL_UP, true);
        updateInputMap(InputEvent::SCROLL_DOWN, false);
    } else if (y < 0) {
        updateInputMap(InputEvent::SCROLL_UP, false);
        updateInputMap(InputEvent::SCROLL_DOWN, true);
    } else {
        updateInputMap(InputEvent::SCROLL_UP, false);
        updateInputMap(InputEvent::SCROLL_DOWN, false);
    }
}

void InputState::registerCallbacks(GLFWwindow* window) {
    glfwSetKeyCallback(window, InputState::keyCallback);
    glfwSetMouseButtonCallback(window, InputState::mouseButtonCallback);
    glfwSetCursorPosCallback(window, InputState::cursorPositionCallback);
    glfwSetScrollCallback(window, InputState::scrollCallback);
}

void InputState::updateInputMap(const InputEvent event, const bool state) {
    inputMap[event].previous = inputMap[event].current;
    inputMap[event].current = state;
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

void InputState::scrollCallback(GLFWwindow* window, double x, double y) {
    if (_instance != nullptr) {
        _instance->updateScroll(x, y);
    }
}

void InputState::postUpdate() {
    for (auto const& [key, event]: InputEventMap) {
        auto state = inputMap[event];
        inputMap[event] = ButtonState(state.current, state.current);
    }

    inputMap[InputEvent::SCROLL_UP] = ButtonState(false, false);
    inputMap[InputEvent::SCROLL_DOWN] = ButtonState(false, false);
}
