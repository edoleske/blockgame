#ifndef BLOCKGAME_CHUNK_H
#define BLOCKGAME_CHUNK_H


#include "common.h"
#include "utils/vertex.h"
#include "gl/vertexBuffer.h"
#include "gl/vertexArray.h"
#include "gl/elementBuffer.h"
#include "block.h"

template<class T>
using ChunkData = array<T, CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z>;

class Chunk;
typedef unordered_map<pair<int, int>, unique_ptr<Chunk>, IntPairHash> ChunkMap;

enum class ChunkState : uint8_t {
    EMPTY, POPULATED, BUILT, UNLOADED
};

class Chunk {
public:
    Chunk(int x, int z, const shared_ptr<ElementBuffer>& ebo, const shared_ptr<BlockTexture>& blockTexture);

    void render();

    void renderTransparent();

    void buildMesh(const ChunkMap& chunkMap);

    void write(vector<char>& data);

    void load(ifstream& in);

    Block getBlock(int x, int y, int z) const;

    void setBlock(int x, int y, int z, const Block& block);

    ChunkState getChunkState() const;

    void setChunkState(ChunkState state);

    const glm::ivec3& getChunkPosition() const;

private:
    ChunkState state = ChunkState::EMPTY;
    glm::ivec3 chunkPosition;
    ChunkData<Block> blocks;

    vector<Vertex> vertices;
    VertexArray vao;
    VertexBuffer vbo;

    vector<Vertex> transparentVertices;
    VertexArray transparentVAO;
    VertexBuffer transparentVBO;

    shared_ptr<ElementBuffer> ebo;
    shared_ptr<BlockTexture> blockTexture;

    void addFace(const Block& block, BlockFace face, const u8vec3& position, bool transparent = false);
    void addBillboard(const Block& block, const u8vec3& position);

    static inline int getIndex(int x, int y, int z);
};


#endif //BLOCKGAME_CHUNK_H
