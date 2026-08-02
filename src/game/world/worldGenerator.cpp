#include "worldGenerator.h"

#include "log.h"
#include "game/block/blockDictionary.h"

WorldGenerator::WorldGenerator(unsigned int seed) : seed(seed), generator(seed) {
    LOG_DEBUG("Initializing world with seed {}", seed);
    noise = make_unique<NoiseGenerator>(seed);

    waterID = lookupRequiredBlock("Water");
    grassID = lookupRequiredBlock("Grass");
    dirtID = lookupRequiredBlock("Dirt");
    stoneID = lookupRequiredBlock("Stone");
    bedrockID = lookupRequiredBlock("Bedrock");
    flowerID = lookupRequiredBlock("Flower");
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
                auto blockID = getBlockType(by, height);

                // Add flowers
                if (by == height + 1 && blockID == 0) {
                    blockID = flowerMap(fx, fz) ? flowerID : blockID;
                }

                chunk->setBlock(bx, by, bz, Block(blockID));
            }
        }
    }

    chunk->setChunkState(ChunkState::POPULATED);
}

int WorldGenerator::getHeight(const float x, const float z) const {
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

bool WorldGenerator::flowerMap(const float x, const float z) {
    auto value = noise->get(x * 2.02f, 2.02f, z * 2.02f);

    return value > 0.75f && distribution(generator) > 0.8f;
}

BlockID WorldGenerator::getBlockType(const int blockHeight, const int terrainHeight) const {
    if (blockHeight > terrainHeight) {
        return blockHeight <= seaLevel ? waterID : 0;
    }
    if (blockHeight == terrainHeight) {
        return terrainHeight > seaLevel - 1 ? grassID : dirtID;
    }
    if (blockHeight < terrainHeight && blockHeight > terrainHeight - 3) {
        return dirtID;
    }
    if (blockHeight > 0 && blockHeight < terrainHeight - 2) {
        return stoneID;
    }
    return bedrockID;
}

BlockID WorldGenerator::lookupRequiredBlock(const string& name) {
    auto id = BlockDictionary::getInstance()->lookup(name);

    if (!id) {
        LOG_WARN("Required block {} could not be found", name);
    }

    return id;
}
