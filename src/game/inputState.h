#ifndef BLOCKGAME_INPUTSTATE_H
#define BLOCKGAME_INPUTSTATE_H


#include "common.h"
#include "inputEvent.h"

struct KeyState {
    bool current, previous;
};

class InputState {
public:
    InputState();
    ~InputState();

    KeyState getState(InputEvent event) const;

    void updateKey(int key, bool state);

    static void registerCallback(GLFWwindow* window);
private:
    unordered_map<int, KeyState> keyStateMap;

    static void callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static InputState* _instance;
};


#endif //BLOCKGAME_INPUTSTATE_H
