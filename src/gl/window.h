#ifndef BLOCKGAME_WINDOW_H
#define BLOCKGAME_WINDOW_H


#include "../common.h"

class Window {
public:
    Window(int width, int height);
    ~Window();

    bool isInitialized() const;
    virtual void loop() = 0;
protected:
    int width, height, version;
    float aspectRatio, time = 0.0f, deltaTime = 0.0f;
    GLFWwindow* window;

    static void framebuffer_resize_callback(GLFWwindow* window, int width, int height);
};


#endif //BLOCKGAME_WINDOW_H
