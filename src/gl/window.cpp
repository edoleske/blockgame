#include "window.h"

#include "log.h"

Window::Window(const int width, const int height) : width(width), height(height) {
    aspectRatio = static_cast<float>(width) / static_cast<float>(height);

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#if defined(DEBUG)
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

    window = glfwCreateWindow(width, height, "BlockGame", nullptr, nullptr);
    if (window == nullptr) {
        LOG_ERROR("Failed to create GLFW window");
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSwapInterval(1);

    // Load OpenGL functions
    initialized = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
    if (!initialized) {
        LOG_ERROR("Failed to initialize OpenGL context");
        glfwTerminate();
        return;
    }

    // Initialize OpenGL debug context if flags are set
    int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(messageCallback, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
    }

    // Register user pointer for static callback
    glfwSetWindowUserPointer(window, this);

    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);

    version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    LOG_INFO("Loaded OpenGL Version: {}", version);
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

void Window::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);

    if (auto* instance = static_cast<Window*>(glfwGetWindowUserPointer(window))) {
        instance->updateWindowSize(width, height);
    }
}

void Window::messageCallback(
    const GLenum source, GLenum, GLuint, GLenum severity, GLsizei, const GLchar* message, const void*) {
    string sSource = source == GL_DEBUG_SOURCE_API
                         ? "API"
                         : source == GL_DEBUG_SOURCE_WINDOW_SYSTEM
                         ? "WIN"
                         : source == GL_DEBUG_SOURCE_SHADER_COMPILER
                         ? "SHD"
                         : source == GL_DEBUG_SOURCE_THIRD_PARTY
                         ? "THP"
                         : source == GL_DEBUG_SOURCE_APPLICATION
                         ? "APP"
                         : "OTH";

    switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:
        LOG_ERROR("[{}] {}", sSource, message);
        DEBUGBREAK();
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        LOG_WARN("[{}] {}", sSource, message);
        break;
    case GL_DEBUG_SEVERITY_LOW:
        LOG_INFO("[{}] {}", sSource, message);
        break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        LOG_DEBUG("[{}] {}", sSource, message);
        break;
    default:
        LOG_WARN("Unrecognized GLenum severity: {}", severity);
        LOG_DEBUG("[{}] {}", sSource, message);
        break;
    }
}
