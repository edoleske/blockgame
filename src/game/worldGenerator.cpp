#include "worldGenerator.h"

WorldGenerator::WorldGenerator(unsigned int seed) : seed(seed), generator(seed) {
    noise = make_unique<NoiseGenerator>(seed);
}

void WorldGenerator::generate(Chunk* chunk) {
    for (int bx = 0; bx < CHUNK_SIZE_X; ++bx) {
        auto fx = static_cast<float>(chunk->getChunkPosition().x) + (static_cast<float>(bx + 1) / CHUNK_SIZE_X) +
                  1.0f / (2 * CHUNK_SIZE_X);
        for (int bz = 0; bz < CHUNK_SIZE_Z; ++bz) {
            auto fz = static_cast<float>(chunk->getChunkPosition().z) + (static_cast<float>(bz + 1) / CHUNK_SIZE_Z) +
                      1.0f / (2 * CHUNK_SIZE_X);
            auto height = getHeight(fx, fz);

            // Populate column
            for (int by = 0; by < CHUNK_SIZE_Y; ++by) {
                auto type = getBlockType(by, height);

                // Add flowers
                if (by == height + 1 && type == BlockType::AIR) {
                    type = flowerMap(fx, fz) ? BlockType::FLOWER : type;
                }

                chunk->setBlock(bx, by, bz, Block(type));
            }
        }
    }

    chunk->setChunkState(ChunkState::POPULATED);
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

bool WorldGenerator::flowerMap(float x, float z) {
    auto value = noise->get(x * 2.02f, 2.02f, z * 2.02f);

    return value > 0.75f && distribution(generator) > 0.8f;
}

BlockType WorldGenerator::getBlockType(int blockHeight, int terrainHeight) {
    if (blockHeight > terrainHeight) {
        return blockHeight <= seaLevel ? BlockType::WATER : BlockType::AIR;
    }
    if (blockHeight == terrainHeight) {
        return terrainHeight > seaLevel - 1 ? BlockType::GRASS : BlockType::DIRT;
    }
    if (blockHeight < terrainHeight && blockHeight > terrainHeight - 3) {
        return BlockType::DIRT;
    }
    if (blockHeight > 0 && blockHeight < terrainHeight - 2) {
        return BlockType::STONE;
    }
    return BlockType::BEDROCK;
}
