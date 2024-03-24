#include "worldGenerator.h"

WorldGenerator::WorldGenerator(unsigned int seed) : seed(seed) {
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

BlockType WorldGenerator::getBlockType(int blockHeight, int terrainHeight) {
    if (blockHeight > terrainHeight) {
        return BlockType::AIR;
    }
    if (blockHeight == terrainHeight) {
        return terrainHeight > 62 ? BlockType::GRASS : BlockType::DIRT;
    }
    if (blockHeight < terrainHeight && blockHeight > terrainHeight - 3) {
        return BlockType::DIRT;
    }
    if (blockHeight > 0 && blockHeight < terrainHeight - 2) {
        return BlockType::STONE;
    }
    return BlockType::BEDROCK;
}
