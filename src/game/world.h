#ifndef BLOCKGAME_WORLD_H
#define BLOCKGAME_WORLD_H


#include "common.h"
#include "chunk.h"
#include "gl/shader.h"
#include "utils/noiseGenerator.h"

class World {
public:
    World();

    Chunk* getChunk(int x, int z) const;

    bool chunkExists(int x, int z) const;

    Block* getBlock(int x, int y, int z);

    void generateSpawnArea() {
        // Create chunks
        for (int x = -SPAWN_SIZE; x <= SPAWN_SIZE; ++x) {
            for (int z = -SPAWN_SIZE; z <= SPAWN_SIZE; ++z) {
                chunkMap.emplace(make_pair(x, z), make_unique<Chunk>(x, z, ebo, blockTexture, noise.get()));
            }
        }

        // Build chunk meshes
        for (int x = -SPAWN_SIZE; x <= SPAWN_SIZE; ++x) {
            for (int z = -SPAWN_SIZE; z <= SPAWN_SIZE; ++z) {
                auto chunk = chunkMap.at(make_pair(x, z)).get();
                chunk->buildMesh(chunkMap);
            }
        }
    }

    void createChunk(int x, int z) {
        chunkMap.emplace(make_pair(x, z), make_unique<Chunk>(x, z, ebo, blockTexture, noise.get()));
        auto chunk = chunkMap.at(make_pair(x, z)).get();
        chunk->buildMesh(chunkMap);

        // Rebuild surrounding meshes
        auto leftChunk = getChunk(x - 1, z);
        if (leftChunk != nullptr) {
            leftChunk->buildMesh(chunkMap);
        }

        auto rightChunk = getChunk(x + 1, z);
        if (rightChunk != nullptr) {
            rightChunk->buildMesh(chunkMap);
        }

        auto backChunk = getChunk(x, z - 1);
        if (backChunk != nullptr) {
            backChunk->buildMesh(chunkMap);
        }

        auto frontChunk = getChunk(x, z + 1);
        if (frontChunk != nullptr) {
            frontChunk->buildMesh(chunkMap);
        }
    }

    void unloadChunk(int x, int z) {
        auto it = chunkMap.find(make_pair(x, z));
        if (it != chunkMap.end()) {
            chunkMap.erase(it);
        }

        // Rebuild surrounding meshes
        auto leftChunk = getChunk(x - 1, z);
        if (leftChunk != nullptr) {
            leftChunk->buildMesh(chunkMap);
        }

        auto rightChunk = getChunk(x + 1, z);
        if (rightChunk != nullptr) {
            rightChunk->buildMesh(chunkMap);
        }

        auto backChunk = getChunk(x, z - 1);
        if (backChunk != nullptr) {
            backChunk->buildMesh(chunkMap);
        }

        auto frontChunk = getChunk(x, z + 1);
        if (frontChunk != nullptr) {
            frontChunk->buildMesh(chunkMap);
        }
    }

    void renderWorld(Shader* shader) {
        shader->use();

        for (auto const& chunkRecord: chunkMap) {
            auto chunk = chunkRecord.second.get();
            shader->setInteger("chunkX", chunk->getChunkPosition().x);
            shader->setInteger("chunkZ", chunk->getChunkPosition().z);

            chunk->render();
        }
    }

private:
    map<pair<int, int>, unique_ptr<Chunk>> chunkMap;
    shared_ptr<BlockTexture> blockTexture;
    unsigned seed;
    unique_ptr<NoiseGenerator> noise;

    const int SPAWN_SIZE = 6;

    // Element buffer object shared between all chunks
    shared_ptr<ElementBuffer> ebo;

    void initializeEBO();
};


#endif //BLOCKGAME_WORLD_H
