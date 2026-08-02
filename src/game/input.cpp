#include "input.h"

#include "log.h"
#include "settings.h"

Input* Input::_instance = nullptr;

Input::Input() : currentCursor(0.0, 0.0), previousCursor(0.0, 0.0) {
    LOG_DEBUG("Initializing input system");
    _instance = this;

    for (auto const& [key, event]: Settings::keymap) {
        inputMap[event] = ButtonState();
    }

    // Scroll inputs are not mapped to a GLFW button, so we track it more manually
    inputMap[Event::SCROLL_UP] = ButtonState();
    inputMap[Event::SCROLL_DOWN] = ButtonState();
}

Input::~Input() {
    LOG_DEBUG("Destructing input system");
    if (_instance == this) {
        _instance = nullptr;
    }
}

Input* Input::getInstance() {
    return _instance;
}

bool Input::isPressed(const Event event) const {
    auto [current, previous] = inputMap.at(event);
    return current && !previous;
}

bool Input::isReleased(const Event event) const {
    auto [current, previous] = inputMap.at(event);
    return !current && previous;
}

ButtonState Input::getState(const Event event) const {
    return inputMap.at(event);
}

vec2 Input::getCursorOffset() {
    auto offset = vec2(static_cast<float>(currentCursor.x - previousCursor.x),
                       static_cast<float>(previousCursor.y - currentCursor.y));
    previousCursor = currentCursor;

    return offset;
}

void Input::updateKey(int key, bool state) {
    if (!Settings::keymap.contains(key)) return;

    auto event = Settings::keymap[key];
    updateInputMap(event, state);
}

void Input::updateCursor(double x, double y) {
    currentCursor = glm::vec<2, double>(x, y);
}

void Input::updateScroll(double x, double y) {
    if (y > 0) {
        updateInputMap(Event::SCROLL_UP, true);
        updateInputMap(Event::SCROLL_DOWN, false);
    } else if (y < 0) {
        updateInputMap(Event::SCROLL_UP, false);
        updateInputMap(Event::SCROLL_DOWN, true);
    } else {
        updateInputMap(Event::SCROLL_UP, false);
        updateInputMap(Event::SCROLL_DOWN, false);
    }
}

void Input::registerCallbacks(GLFWwindow* window) {
    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPositionCallback);
    glfwSetScrollCallback(window, scrollCallback);
}

void Input::updateInputMap(const Event event, const bool state) {
    inputMap[event].previous = inputMap[event].current;
    inputMap[event].current = state;
}

void Input::keyCallback(GLFWwindow* window, const int key, int, const int action, int) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (_instance != nullptr) {
        _instance->updateKey(key, action != GLFW_RELEASE);
    }
}

void Input::mouseButtonCallback(GLFWwindow*, const int button, const int action, int) {
    if (_instance != nullptr) {
        // Mouse button codes (0-8) are compatible with key codes (32-348)
        _instance->updateKey(button, action == GLFW_PRESS);
    }
}

void Input::cursorPositionCallback(GLFWwindow*, const double x, const double y) {
    if (_instance != nullptr) {
        _instance->updateCursor(x, y);
    }
}

void Input::scrollCallback(GLFWwindow*, const double x, const double y) {
    if (_instance != nullptr) {
        _instance->updateScroll(x, y);
    }
}

void Input::postUpdate() {
    for (auto const& [key, event]: Settings::keymap) {
        auto [current, previous] = inputMap[event];
        inputMap[event] = ButtonState(current, current);
    }

    inputMap[Event::SCROLL_UP] = ButtonState(false, false);
    inputMap[Event::SCROLL_DOWN] = ButtonState(false, false);
}
