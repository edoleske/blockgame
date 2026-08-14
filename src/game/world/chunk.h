#ifndef BLOCKGAME_CHUNK_H
#define BLOCKGAME_CHUNK_H


#include "common.h"
#include "utils/vertex.h"
#include "gl/vertexBuffer.h"
#include "gl/vertexArray.h"
#include "gl/elementBuffer.h"
#include "game/block/block.h"
#include "game/block/palettedBlockData.h"
#include "utils/regionFile.h"

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

    void write(RegionFile* regionFile) const;

    void load(RegionFile* regionFile);

    BlockID getBlock(int x, int y, int z) const;

    void setBlock(int x, int y, int z, const BlockID& block);

    ChunkState getChunkState() const;

    void setChunkState(ChunkState newState);

    const ivec3& getChunkPosition() const;

    int getVertexCount() const;

    static bool isValidBlockPosition(int x, int y, int z);

private:
    ChunkState state = ChunkState::EMPTY;
    ivec3 chunkPosition;
    PalettedBlockData data;

    VertexArray vao;
    VertexBuffer vbo;
    int vertexCount = 0;

    VertexArray transparentVAO;
    VertexBuffer transparentVBO;
    int transparentVertexCount = 0;

    static void addFace(
        vector<Vertex>& vertices, vector<Vertex>& transparentVertices, const Block& block, BlockFace face,
        const u8vec3& position);

    static void addBillboard(
        vector<Vertex>& vertices, vector<Vertex>& transparentVertices, const Block& block, const u8vec3& position);

    static inline int getIndex(int x, int y, int z);

    static inline bool isVisibleFace(const Block& a, const Block& b);
};


#endif //BLOCKGAME_CHUNK_H
