#include "game.h"

#include "log.h"
#include "toml++/toml.h"

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

    initializeBlocks();

    world = make_unique<World>();
    world->generateSpawnArea();

    uiRenderer = make_unique<UIRenderer>();
    uiRenderer->updateWindowSize(width, height);

    Input::registerCallbacks(window);
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

        world->renderWorld(player.getCamera(), textureArray);

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

// Load textures and connect block types to those textures
// Returns generated texture array for binding and destructing
void Game::initializeBlocks() {
    LOG_DEBUG("Initializing block definitions");

    std::hash<BlockType> blockHasher;
    unordered_map<string, uint16_t> textureLayerMap = {};

    const auto table = toml::parse_file("../resources/data.toml");

    if (auto blocks = table["block"].as_array()) {
        for (auto& node : *blocks) {
            if (auto blockTable = node.as_table()) {
                BlockType type;

                auto name = (*blockTable)["name"].value<string>();
                auto opaque = (*blockTable)["opaque"].value_or(true);
                auto billboard = (*blockTable)["billboard"].value_or(type.isBillboard);
                auto all = (*blockTable)["all"].value<string>();
                auto side = (*blockTable)["side"].value<string>();
                auto top = (*blockTable)["top"].value<string>();
                auto bottom = (*blockTable)["bottom"].value<string>();

                auto isValid = name.has_value() && (all.has_value() ||
                    (side.has_value() && top.has_value() && bottom.has_value()));
                if (!isValid) throw std::runtime_error("Invalid block loaded");

                for (auto filename : {all, side, top, bottom}) {
                    if (filename.has_value() && !textureLayerMap.contains(filename.value())) {
                        textureLayerMap[filename.value()] = textureLayerMap.size();
                    }
                }

                type.name = name.value();
                type.id = blockHasher(type);
                type.opaque = opaque;
                type.isBillboard = billboard;

                if (all.has_value()) {
                    for (auto& faceTexture : type.faceTextures) {
                        faceTexture = textureLayerMap[all.value()];
                    }
                }

                if (side.has_value()) {
                    for (int i = 2; i < type.faceTextures.size(); i++) {
                        type.faceTextures[i] = textureLayerMap[side.value()];
                    }
                }

                if (top.has_value()) {
                    type.faceTextures[0] = textureLayerMap[top.value()];
                }

                if (bottom.has_value()) {
                    type.faceTextures[1] = textureLayerMap[bottom.value()];
                }

                blockDictionary.insert(type);
            }
        }
    }

    if (auto blockCount = blockDictionary.count(); blockCount > 0) {
        LOG_INFO("Loaded {} blocks into dictionary", blockCount);
    } else {
        LOG_ERROR("Failed to load any blocks");
    }

    initializeTextureArray(textureLayerMap);
}

void Game::initializeTextureArray(const unordered_map<string, uint16_t>& textureLayerMap) {
    LOG_DEBUG("Creating texture array with {} layers", textureLayerMap.size());
    textureArray.allocate(16, static_cast<int>(textureLayerMap.size()));
    TextureArray::setParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
    TextureArray::setParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
    TextureArray::setParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    TextureArray::setParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    vector<string> filenames(textureLayerMap.size());
    for (const auto& [filename, index] : textureLayerMap) {
        filenames[index] = filename;
    }

    for (const auto& filename : filenames) {
        textureArray.addLayer("../resources/textures/" + filename);
    }
}
