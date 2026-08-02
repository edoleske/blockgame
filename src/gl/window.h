#ifndef BLOCKGAME_WINDOW_H
#define BLOCKGAME_WINDOW_H


#include "common.h"

class Window {
public:
    Window(int width, int height);
    virtual ~Window();

    bool isInitialized() const;
    virtual void loop() = 0;
    virtual void updateWindowSize(int w, int h);
protected:
    int width, height, initialized;
    float aspectRatio, time = 0.0f, deltaTime = 0.0f;
    string version;
    GLFWwindow* window;

    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
    static void messageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
};


#endif //BLOCKGAME_WINDOW_H
