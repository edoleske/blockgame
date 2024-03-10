cmake_minimum_required(VERSION 3.27)
project(blockgame)

set(CMAKE_CXX_STANDARD 20)

find_package(glfw3 REQUIRED)

include_directories(external/glad/include external src)

add_executable(blockgame src/main.cpp external/glad/src/gl.c
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
        src/game/camera.cpp
        src/game/camera.h
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
)

target_link_libraries(blockgame glfw)