#ifndef BLOCKGAME_INPUTSTATE_H
#define BLOCKGAME_INPUTSTATE_H


#include "common.h"
#include "inputEvent.h"

struct ButtonState {
    bool current, previous;
};

class InputState {
public:
    InputState();
    ~InputState();

    ButtonState getState(InputEvent event) const;

    vec2 getCursorOffset();

    void updateKey(int key, bool state);

    void updateCursor(double x, double y);

    // Called every frame to update keys so previous frame is accurate
    void postUpdate();

    static void registerCallbacks(GLFWwindow* window);
private:
    unordered_map<InputEvent, ButtonState> inputMap;
    vec<2, double> currentCursor, previousCursor;

    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void cursorPositionCallback(GLFWwindow* window, double x, double y);

    // Created InputState instance for registering static callbacks with GLFW
    static InputState* _instance;
};


#endif //BLOCKGAME_INPUTSTATE_H
