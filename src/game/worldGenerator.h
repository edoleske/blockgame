#ifndef BLOCKGAME_WORLDGENERATOR_H
#define BLOCKGAME_WORLDGENERATOR_H


#include "common.h"
#include "utils/noiseGenerator.h"

class WorldGenerator {
public:
    WorldGenerator(unsigned seed);

    int getHeight(float x, float z);
private:
    unsigned seed;
    unique_ptr<NoiseGenerator> noise;

    const int octaves = 5;
    const float lacunarity = 1.75f;
    const float gain = 0.1f;
};


#endif //BLOCKGAME_WORLDGENERATOR_H
