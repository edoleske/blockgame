#ifndef BLOCKGAME_INPUTSTATE_H
#define BLOCKGAME_INPUTSTATE_H

#include "common.h"

struct ButtonState {
    bool current, previous;
};


class Input {
public:
    Input();
    ~Input();

    static Input* getInstance();

    enum class Event {
        MOVE_LEFT, MOVE_RIGHT, MOVE_FRONT, MOVE_BACK,
        MOVE_UP, MOVE_DOWN, SPRINT, TOGGLE_FLY,
        MINE_BLOCK, PLACE_BLOCK, SCROLL_UP, SCROLL_DOWN,
        TOGGLE_DEBUG,
        ITEM_1, ITEM_2, ITEM_3, ITEM_4, ITEM_5, ITEM_6, ITEM_7, ITEM_8, ITEM_9, ITEM_0
    };

    bool isPressed(Event event) const;

    bool isReleased(Event event) const;

    ButtonState getState(Event event) const;

    vec2 getCursorOffset();

    void updateKey(int key, bool state);

    void updateCursor(double x, double y);

    void updateScroll(double x, double y);

    // Called every frame to update keys so previous frame is accurate
    void postUpdate();

    static void registerCallbacks(GLFWwindow* window);
private:
    unordered_map<Event, ButtonState> inputMap;
    vec<2, double> currentCursor, previousCursor;

    void updateInputMap(Event event, bool state);

    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void cursorPositionCallback(GLFWwindow* window, double x, double y);
    static void scrollCallback(GLFWwindow* window, double x, double y);

    // Created InputState instance for registering static callbacks with GLFW
    static Input* _instance;
};


#endif //BLOCKGAME_INPUTSTATE_H
