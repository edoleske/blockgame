#ifndef BLOCKGAME_WORLD_H
#define BLOCKGAME_WORLD_H


#include "common.h"
#include "chunk.h"
#include "gl/shader.h"
#include "worldGenerator.h"

class World {
public:
    World(const unique_ptr<Shader>& shader);

    Chunk* getChunk(int x, int z) const;

    bool chunkExists(int x, int z) const;

    optional<Block> getBlock(int x, int y, int z) const;

    optional<Block> getBlockRaycast(vec3 position, const vec3 &front, float distance) const;

    void generateSpawnArea();

    void updateChunks(const vec3& playerPosition);

    void loadChunk(int x, int z);

    void unloadChunk(int x, int z);

    void renderWorld(Shader* shader, const vec3& playerPosition);

private:
    string name = "NewWorld";
    ChunkMap chunkMap;
    unsigned seed;
    unique_ptr<WorldGenerator> worldGen;

    const int SPAWN_SIZE = RENDER_DISTANCE;

    // Shared block texture
    shared_ptr<BlockTexture> blockTexture;

    // Element buffer object shared between all chunks
    shared_ptr<ElementBuffer> ebo;

    void initializeEBO();

    inline void unbuildChunk(int x, int z);
    inline bool chunkNeighborsPopulated(int x, int z) const;

    // Region file management
    void createLevel();
    bool loadLevel();
    inline string getRegionFilePath(int x, int z) const;
    static void createRegionFile(const string& filepath);
    void updateRegionFile(int x, int z) const;
    void loadFromRegionFile(int x, int z);
};


#endif //BLOCKGAME_WORLD_H
