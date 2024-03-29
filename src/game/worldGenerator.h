#ifndef BLOCKGAME_WORLDGENERATOR_H
#define BLOCKGAME_WORLDGENERATOR_H


#include "common.h"
#include "utils/noiseGenerator.h"
#include "block.h"
#include "chunk.h"

class WorldGenerator {
public:
    explicit WorldGenerator(unsigned seed);

    void generate(Chunk* chunk);
private:
    unsigned seed;
    unique_ptr<NoiseGenerator> noise;

    // RNG for features like flowers
    std::mt19937 generator;
    std::uniform_real_distribution<> distribution {0.0, 1.0 };

    static const int seaLevel = 62;
    const int octaves = 5;
    const float lacunarity = 1.75f;
    const float gain = 0.1f;

    int getHeight(float x, float z);

    bool flowerMap(float x, float z);

    static BlockType getBlockType(int blockHeight, int terrainHeight);
};


#endif //BLOCKGAME_WORLDGENERATOR_H
