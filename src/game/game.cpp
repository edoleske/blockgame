#include "game.h"

Game::Game(const int width, const int height) : Window(width, height),
                                    player(45.0f, static_cast<float>(width) / static_cast<float>(height)) {
    // Turn on depth testing
    glEnable(GL_DEPTH_TEST);

    // Cull the front face of the cube (faces inward)
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    // Alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.6f, 0.8f, 0.9f, 1.0f);

    // Line width for highlight
    glLineWidth(2.0f);

    // Capture cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);

    // Disable VSync for performance tuning
    glfwSwapInterval(0);

    // Wireframe for mesh debugging
    //    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    world = make_unique<World>();
    world->generateSpawnArea();

    uiRenderer = make_unique<UIRenderer>();
    uiRenderer->updateWindowSize(width, height);

    InputState::registerCallbacks(window);
}

void Game::loop() {
    // First update setup
    time = static_cast<float>(glfwGetTime());

    while (!glfwWindowShouldClose(window)) {
        auto currentTime = static_cast<float>(glfwGetTime());
        deltaTime = currentTime - time;
        time = currentTime;
        glfwSetWindowTitle(window, std::to_string(1.0f / deltaTime).c_str());

        world->updateChunks(player.getCamera().getPosition());

        // Poll input and update game based on input state
        glfwPollEvents();
        update();
        input.postUpdate();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        world->renderWorld(player.getCamera());

        // Render UI
        uiRenderer->render();

        glfwSwapBuffers(window);
    }
}

void Game::updateWindowSize(int w, int h) {
    Window::updateWindowSize(w, h);
    player.updateAspectRatio(aspectRatio);
    uiRenderer->updateWindowSize(w, h);
}

void Game::update() {
    player.update(deltaTime, world);
    uiRenderer->update(deltaTime, player);
}
