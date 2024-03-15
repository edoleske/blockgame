#ifndef BLOCKGAME_NOISEGENERATOR_H
#define BLOCKGAME_NOISEGENERATOR_H


#include "common.h"

class NoiseGenerator {
public:
    NoiseGenerator(unsigned seed);

    float get(float x, float y, float z) const;
private:
    std::mt19937 generator;
    array<uint8_t, 256> permutations;

    inline constexpr float fade(float t) const;
    inline constexpr float lerp(float t, float a, float b) const;
    inline constexpr float grad(uint8_t hash, float x, float y, float z) const;
};


#endif //BLOCKGAME_NOISEGENERATOR_H
