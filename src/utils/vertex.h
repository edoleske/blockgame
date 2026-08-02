#ifndef BLOCKGAME_VERTEX_H
#define BLOCKGAME_VERTEX_H

#include "common.h"


struct Vertex {
    u8vec3 position;
    u8vec2 uv;
    uint8_t layer = 0;
};

#endif //BLOCKGAME_VERTEX_H
