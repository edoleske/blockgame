cmake_minimum_required(VERSION 3.24)
project(blockgame
        VERSION 0.0.0
        DESCRIPTION "Minecraft-inspired OpenGL game"
        HOMEPAGE_URL "https://github.com/edoleske/blockgame")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Fetch/find project dependencies
include(FetchContent)

FetchContent_Declare(
        glfw3
        GIT_REPOSITORY https://github.com/glfw/glfw
        GIT_TAG d9d6f0f1f967807ffade6598ea9a631ebaf37a56
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
        #FIND_PACKAGE_ARGS
)

# Silence GLAD CMake Version Error
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

FetchContent_Declare(
        glad
        GIT_REPOSITORY https://github.com/Dav1dde/glad
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
)
set(GLAD_PROFILE "core" CACHE STRING "OpenGL profile")
set(GLAD_GENERATOR "c" CACHE STRING "Language to generate the binding for")

FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 1.0.1
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
        FIND_PACKAGE_ARGS)

FetchContent_Declare(
        tomlplusplus
        GIT_REPOSITORY https://github.com/marzer/tomlplusplus
        GIT_TAG v3.4.0
)

FetchContent_Declare(
        stb_image
        GIT_REPOSITORY https://github.com/nothings/stb
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
        FIND_PACKAGE_ARGS
)

FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog
        GIT_TAG v1.x
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
        FIND_PACKAGE_ARGS
)

FetchContent_MakeAvailable(glfw3 glad glm stb_image spdlog tomlplusplus)

add_library(stb_image INTERFACE ${stb_image_SOURCE_DIR}/stb_image.h)
target_include_directories(stb_image INTERFACE ${stb_image_SOURCE_DIR})

# Create project target
add_executable(${PROJECT_NAME} src/main.cpp
        src/common.h
        src/gl/window.cpp
        src/gl/window.h
        src/game/game.cpp
        src/game/game.h
        src/gl/vertexArray.cpp
        src/gl/vertexArray.h
        src/gl/vertexBuffer.cpp
        src/gl/vertexBuffer.h
        src/utils/vertex.h
        src/gl/shader.cpp
        src/gl/shader.h
        src/game/player/camera.cpp
        src/game/player/camera.h
        src/game/block/block.cpp
        src/game/block/block.h
        src/game/world/chunk.cpp
        src/game/world/chunk.h
        src/game/world/world.cpp
        src/game/world/world.h
        src/gl/texture.cpp
        src/gl/texture.h
        src/gl/elementBuffer.cpp
        src/gl/elementBuffer.h
        src/utils/noiseGenerator.cpp
        src/utils/noiseGenerator.h
        src/game/player/player.cpp
        src/game/player/player.h
        src/game/world/worldGenerator.cpp
        src/game/world/worldGenerator.h
        src/game/input.cpp
        src/game/input.h
        src/ui/uiRenderer.cpp
        src/ui/uiRenderer.h
        src/ui/uiElement.cpp
        src/ui/uiElement.h
        src/ui/elements/crosshair.cpp
        src/ui/elements/crosshair.h
        src/ui/uiBatch.cpp
        src/ui/uiBatch.h
        src/ui/font.cpp
        src/ui/font.h
        src/ui/elements/textBox.cpp
        src/ui/elements/textBox.h
        src/ui/uiTextureAtlas.cpp
        src/ui/uiTextureAtlas.h
        src/ui/elements/toolbar.cpp
        src/ui/elements/toolbar.h
        src/game/player/inventory.cpp
        src/game/player/inventory.h
        src/game/settings.cpp
        src/game/settings.h
        src/game/item/item.cpp
        src/game/item/item.h
        src/game/item/itemStack.cpp
        src/game/item/itemStack.h
        src/game/item/itemBlock.cpp
        src/game/item/itemBlock.h
        src/game/block/blockType.cpp
        src/game/block/blockType.h
        src/gl/textureArray.cpp
        src/gl/textureArray.h
        src/game/block/blockDictionary.cpp
        src/game/block/blockDictionary.h
        src/log.h
        src/gl/texture2D.cpp
        src/gl/texture2D.h
        src/utils/debug.h
)

target_include_directories(${PROJECT_NAME} PRIVATE src ${tomlplusplus_SOURCE_DIR}/include ${spdlog_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw glm::glm glad stb_image spdlog::spdlog)
target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:DEBUG>)
