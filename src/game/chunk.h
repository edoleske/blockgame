#ifndef BLOCKGAME_CHUNK_H
#define BLOCKGAME_CHUNK_H


#include "../common.h"
#include "../utils/vertex.h"
#include "../gl/vertexBuffer.h"
#include "../gl/vertexArray.h"
#include "block.h"

template <class T>
using ChunkData = array<array<array<T, CHUNK_SIZE_X>, CHUNK_SIZE_Y>, CHUNK_SIZE_Z>;

class Chunk {
public:
    Chunk(int x, int z);

    Block* getBlock(int x, int y, int z) const;

    bool isChunkBuilt() const;

    void render();

    const glm::ivec3 &getChunkPosition() const;

    void buildMesh(Chunk* leftChunk, Chunk* rightChunk, Chunk* backChunk, Chunk* frontChunk);
private:
    bool chunkBuilt = false;
    glm::ivec3 chunkPosition;
    ChunkData<unique_ptr<Block>> blocks;
    vector<Vertex> vertices;
    VertexArray vao;
    VertexBuffer vbo;
};


#endif //BLOCKGAME_CHUNK_H
