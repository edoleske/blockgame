#include "window.h"

Window::Window(int width, int height) : width(width), height(height) {
    aspectRatio = static_cast<float>(width) / static_cast<float>(height);

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, "BlockGame", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSwapInterval(1);

    // Load OpenGL functions
    initialized = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
    if (!initialized) {
        std::cerr << "Failed to initialize OpenGL context" << std::endl;
        glfwTerminate();
        return;
    }

    version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    std::cout << "Loaded OpenGL Version: " << version << std::endl;

    // Register user pointer for static callback
    glfwSetWindowUserPointer(window, this);

    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);
}

Window::~Window() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

bool Window::isInitialized() const {
    return !!initialized;
}

void Window::updateWindowSize(int w, int h) {
    width = w;
    height = h;
    aspectRatio = (static_cast<float>(width) / static_cast<float>(height));
}

void Window::framebuffer_resize_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);

    if (auto* instance = static_cast<Window*>(glfwGetWindowUserPointer(window))) {
        instance->updateWindowSize(width, height);
    }
}
