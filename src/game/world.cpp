#include "world.h"

Chunk* World::getChunk(int x, int z) const {
    if (chunkExists(x, z)) {
        return chunkMap.at(make_pair(x, z)).get();
    }
    return nullptr;
}

bool World::chunkExists(int x, int z) const {
    return chunkMap.find(make_pair(x, z)) != chunkMap.end();
}

Block* World::getBlock(int x, int y, int z) {
    auto chunk = getChunk(x / CHUNK_SIZE_X, z / CHUNK_SIZE_Z);
    if (chunk != nullptr) {
        return chunk->getBlock(x % CHUNK_SIZE_X, y, z % CHUNK_SIZE_Z);
    }
    return nullptr;
}