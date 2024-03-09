#include "world.h"

World::World() {
    // Initialize element buffer object shared between all chunks
    ebo = make_shared<ElementBuffer>();
    initializeEBO();

    blockTexture = make_shared<BlockTexture>();
    blockTexture->getTexture()->bind();
}

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

void World::initializeEBO() {
    // There are 2 triangles per face, so 6 indices per face
    auto faceCount = CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z * 6;
    auto data = new GLuint[faceCount * 6];

    int offset = 0;
    for (size_t i = 0; i < faceCount * 6; i += 6) {
        data[i] = 0 + offset;
        data[i + 1] = 1 + offset;
        data[i + 2] = 2 + offset;
        data[i + 3] = 2 + offset;
        data[i + 4] = 3 + offset;
        data[i + 5] = 0 + offset;

        offset += 4;
    }

    ebo->bufferData(faceCount * 6 * sizeof(GLuint), data);
}
