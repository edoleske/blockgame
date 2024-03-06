#include "game.h"

Game::Game(int width, int height) : Window(width, height) {
    // Turn on depth testing
    glEnable(GL_DEPTH_TEST);

    // Cull the front face of the cube (faces inward)
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    glClearColor(0.6f, 0.8f, 0.9f, 1.0f);

    // Capture cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Wireframe for mesh debugging
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    camera = make_unique<Camera>(45.0f, static_cast<float>(width) / static_cast<float>(height));
    shader = make_unique<Shader>("../resources/shaders/basic.vert", "../resources/shaders/basic.frag");
    world = make_unique<World>();
    world->generateSpawnArea();
}

void Game::loop() {
    while (!glfwWindowShouldClose(window)) {
        auto currentTime = static_cast<float>(glfwGetTime());
        deltaTime = currentTime - time;
        time = currentTime;
        glfwSetWindowTitle(window, std::to_string(deltaTime).c_str());

        glfwPollEvents();
        handleInput();
        handleMouse();

        // Window can resize itself, so we update our class every frame in case it has changed
        glfwGetWindowSize(window, &width, &height);
        camera->setAspectRatio(width, height);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw test thing
        shader->use();
        shader->setMatrix4("view", camera->getView());
        shader->setMatrix4("projection", camera->getProjection());

        world->renderWorld(shader.get());

        glfwSwapBuffers(window);
    }
}

void Game::handleInput() {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    vec2 velocity(0.0f, 0.0f);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        velocity.y += 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        velocity.y -= 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        velocity.x -= 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        velocity.x += 1.0f;
    }

    if (glm::length(velocity) != 0) {
        camera->move(glm::normalize(velocity), deltaTime);
    }
}

void Game::handleMouse() {
    double x, y;
    glfwGetCursorPos(window, &x, &y);

    auto xOffset = static_cast<float>(x - lastMouseX);
    auto yOffset = static_cast<float>(lastMouseY - y);
    lastMouseX = x;
    lastMouseY = y;

    camera->rotate(xOffset, yOffset);
}
