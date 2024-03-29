#ifndef BLOCKGAME_WORLDGENERATOR_H
#define BLOCKGAME_WORLDGENERATOR_H


#include "common.h"
#include "utils/noiseGenerator.h"
#include "block.h"

class WorldGenerator {
public:
    explicit WorldGenerator(unsigned seed);

    int getHeight(float x, float z);

    static BlockType getBlockType(int blockHeight, int terrainHeight);
private:
    unsigned seed;
    unique_ptr<NoiseGenerator> noise;

    static const int seaLevel = 62;
    const int octaves = 5;
    const float lacunarity = 1.75f;
    const float gain = 0.1f;
};


#endif //BLOCKGAME_WORLDGENERATOR_H
