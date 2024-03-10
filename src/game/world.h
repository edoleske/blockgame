#ifndef BLOCKGAME_WORLD_H
#define BLOCKGAME_WORLD_H


#include "common.h"
#include "chunk.h"
#include "gl/shader.h"

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
                chunkMap.emplace(make_pair(x, z), make_unique<Chunk>(x, z, ebo, blockTexture));
            }
        }

        // Build chunk meshes
        for (int x = -SPAWN_SIZE; x <= SPAWN_SIZE; ++x) {
            for (int z = -SPAWN_SIZE; z <= SPAWN_SIZE; ++z) {
                auto chunk = chunkMap.at(make_pair(x, z)).get();

                // Get adjacent chunks
                auto leftChunk = getChunk(x - 1, z);
                auto rightChunk = getChunk(x + 1, z);
                auto backChunk = getChunk(x, z - 1);
                auto frontChunk = getChunk(x, z + 1);

                chunk->buildMesh(leftChunk, rightChunk, backChunk, frontChunk);
            }
        }
    }

    void createChunk(int x, int z) {
        chunkMap.emplace(make_pair(x, z), make_unique<Chunk>(x, z, ebo, blockTexture));
        auto chunk = chunkMap.at(make_pair(x, z)).get();
        chunk->buildMesh(nullptr, nullptr, nullptr, nullptr);
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
    const int SPAWN_SIZE = 2;

    // Element buffer object shared between all chunks
    shared_ptr<ElementBuffer> ebo;

    void initializeEBO();
};


#endif //BLOCKGAME_WORLD_H
