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

    void generateSpawnArea();

    void loadChunk(int x, int z);

    void unloadChunk(int x, int z);

    void renderWorld(Shader* shader);

private:
    string name = "NewWorld";
    map<pair<int, int>, unique_ptr<Chunk>> chunkMap;
    unsigned seed;
    unique_ptr<NoiseGenerator> noise;

    const int SPAWN_SIZE = 6;

    // Shared block texture
    shared_ptr<BlockTexture> blockTexture;

    // Element buffer object shared between all chunks
    shared_ptr<ElementBuffer> ebo;

    void initializeEBO();

    // Region file management
    inline string getRegionFilePath(int x, int z) const;
    static void createRegionFile(const string& filepath);
    void updateRegionFile(int x, int z) const;
    void loadFromRegionFile(int x, int z);
};


#endif //BLOCKGAME_WORLD_H
