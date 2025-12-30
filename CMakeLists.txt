cmake_minimum_required(VERSION 3.10)
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
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
        FIND_PACKAGE_ARGS
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
        stb_image
        GIT_REPOSITORY https://github.com/nothings/stb
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
        FIND_PACKAGE_ARGS)

FetchContent_MakeAvailable(glfw3 glad glm stb_image)

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
        src/player/camera.cpp
        src/player/camera.h
        src/game/block.cpp
        src/game/block.h
        src/game/chunk.cpp
        src/game/chunk.h
        src/game/world.cpp
        src/game/world.h
        src/gl/texture.cpp
        src/gl/texture.h
        src/gl/elementBuffer.cpp
        src/gl/elementBuffer.h
        src/game/blockTexture.cpp
        src/game/blockTexture.h
        src/utils/noiseGenerator.cpp
        src/utils/noiseGenerator.h
        src/player/player.cpp
        src/player/player.h
        src/game/worldGenerator.cpp
        src/game/worldGenerator.h
        src/game/inputState.cpp
        src/game/inputState.h
        src/game/inputEvent.h
)

target_include_directories(${PROJECT_NAME} PRIVATE src)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw glm::glm glad stb_image)
