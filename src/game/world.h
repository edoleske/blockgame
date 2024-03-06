#ifndef BLOCKGAME_WORLD_H
#define BLOCKGAME_WORLD_H


#include "../common.h"
#include "chunk.h"
#include "../gl/shader.h"

class World {
public:
    World() = default;

    Chunk* getChunk(int x, int z) const;
    bool chunkExists(int x, int z) const;

    Block* getBlock(int x, int y, int z);

    void generateSpawnArea() {
        // Create chunks
        for (int x = -1; x <= 1; ++x) {
            for (int z = -1; z <= 1; ++z) {
                    chunkMap.emplace(make_pair(x, z), make_unique<Chunk>(x, z));

            }
        }

        // Build chunk meshes
        for (int x = -1; x <= 1; ++x) {
            for (int z = -1; z <= 1; ++z) {
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
        chunkMap.emplace(make_pair(x, z), make_unique<Chunk>(x, z));
        auto chunk = chunkMap.at(make_pair(x, z)).get();
        chunk->buildMesh(nullptr, nullptr, nullptr, nullptr);
    }

    void renderWorld(Shader* shader) {
        shader->use();

        for (auto const &chunkRecord : chunkMap) {
            auto chunk = chunkRecord.second.get();
            shader->setInteger("chunkX", chunk->getChunkPosition().x);
            shader->setInteger("chunkZ", chunk->getChunkPosition().z);

            chunk->render();
        }
    }
private:
    map<pair<int, int>, unique_ptr<Chunk>> chunkMap;
};


#endif //BLOCKGAME_WORLD_H
