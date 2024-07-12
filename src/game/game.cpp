#include "game.h"

Game::Game(int width, int height) : Window(width, height),
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

    // Capture cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);

    // Disable VSync for performance tuning
    glfwSwapInterval(0);

    // Wireframe for mesh debugging
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    shader = make_unique<Shader>("../resources/shaders/withTexture.vert", "../resources/shaders/withTexture.frag");
    shader->use();
    shader->setInteger("renderDistance", RENDER_DISTANCE);
    shader->setVector4f("uFogColor", vec4((vec3(190.0f, 220.0f, 245.0f) / 255.0f) * 1.1f, 1.0f));

    world = make_unique<World>(shader);
    world->generateSpawnArea();

    InputState::registerCallback(window);
    glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
}

void Game::loop() {
    while (!glfwWindowShouldClose(window)) {
        auto currentTime = static_cast<float>(glfwGetTime());
        deltaTime = currentTime - time;
        time = currentTime;
        glfwSetWindowTitle(window, std::to_string(1.0f / deltaTime).c_str());

        glfwPollEvents();
        handleInput();
        handleMouse();

        // Window can resize itself, so we updateAspectRatio our class every frame in case it has changed
        glfwGetWindowSize(window, &width, &height);
        player.updateAspectRatio(width, height);

        world->updateChunks(player.getCamera().getPosition());

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update shader with camera matrices
        shader->use();
        shader->setMatrix4("view", player.getCamera().getView());
        shader->setMatrix4("projection", player.getCamera().getProjection());

        world->renderWorld(shader.get(), player.getCamera().getPosition());

        glfwSwapBuffers(window);
    }
}

void Game::handleInput() {
    vec3 velocity(0.0f);

    if (input.getState(InputEvent::MOVE_FRONT).current) {
        velocity.z += 1.0f;
    }
    if (input.getState(InputEvent::MOVE_BACK).current) {
        velocity.z -= 1.0f;
    }
    if (input.getState(InputEvent::MOVE_LEFT).current) {
        velocity.x -= 1.0f;
    }
    if (input.getState(InputEvent::MOVE_RIGHT).current) {
        velocity.x += 1.0f;
    }
    if (input.getState(InputEvent::MOVE_UP).current) {
        velocity.y += 1.0f;
    }
    if (input.getState(InputEvent::MOVE_DOWN).current) {
        velocity.y -= 1.0f;
    }

    if (glm::length(velocity) != 0) {
        player.onMove(glm::normalize(velocity) * deltaTime);
    }
}

void Game::handleMouse() {
    double x, y;
    glfwGetCursorPos(window, &x, &y);

    auto xOffset = static_cast<float>(x - lastMouseX);
    auto yOffset = static_cast<float>(lastMouseY - y);
    lastMouseX = x;
    lastMouseY = y;

    player.onRotate(xOffset, yOffset);

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        auto result = world->getBlockRaycast(player.getCamera().getPosition(), player.getCamera().getFront(), 6.0f);
    }
}
