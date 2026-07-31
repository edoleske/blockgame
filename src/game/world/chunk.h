#ifndef BLOCKGAME_CHUNK_H
#define BLOCKGAME_CHUNK_H


#include "common.h"
#include "utils/vertex.h"
#include "gl/vertexBuffer.h"
#include "gl/vertexArray.h"
#include "gl/elementBuffer.h"
#include "../block/block.h"

template <class T>
using ChunkData = array<T, CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z>;

class Chunk;
typedef unordered_map<pair<int, int>, unique_ptr<Chunk>, IntPairHash> ChunkMap;

enum class ChunkState : uint8_t {
    EMPTY, POPULATED, BUILT, UNLOADED
};

class Chunk {
public:
    Chunk(int x, int z, const shared_ptr<ElementBuffer>& ebo);

    void render() const;

    void renderTransparent() const;

    void buildMesh(const ChunkMap& chunkMap);

    void write(vector<char>& data) const;

    void load(ifstream& in);

    Block getBlock(int x, int y, int z) const;

    void setBlock(int x, int y, int z, const Block& block);

    ChunkState getChunkState() const;

    void setChunkState(ChunkState newState);

    const ivec3& getChunkPosition() const;

    static bool isValidBlockPosition(int x, int y, int z);

private:
    ChunkState state = ChunkState::EMPTY;
    ivec3 chunkPosition;
    ChunkData<Block> blocks;

    vector<Vertex> vertices;
    VertexArray vao;
    VertexBuffer vbo;

    vector<Vertex> transparentVertices;
    VertexArray transparentVAO;
    VertexBuffer transparentVBO;

    shared_ptr<ElementBuffer> ebo;

    void addFace(const BlockType& type, BlockFace face, const u8vec3& position);

    void addBillboard(const BlockType& type, const u8vec3& position);

    static inline int getIndex(int x, int y, int z);

    static inline bool isVisibleFace(const BlockType& a, const BlockType& b);
};


#endif //BLOCKGAME_CHUNK_H
