#include "worldGenerator.h"

WorldGenerator::WorldGenerator(unsigned int seed): seed(seed) {
    noise = make_unique<NoiseGenerator>(seed);
}

int WorldGenerator::getHeight(float x, float z) {
    int height = 50;
    float amplitude = 0.75f;
    float frequency = 0.25f;

    for (int i = 0; i < octaves; ++i) {
        height += static_cast<int>(amplitude * noise->get(x * frequency, frequency, z * frequency) * 50);
        frequency *= lacunarity;
        amplitude *= gain;
    }

    return height;
}
