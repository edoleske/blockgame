#ifndef BLOCKGAME_PALETTEDBLOCKDATA_H
#define BLOCKGAME_PALETTEDBLOCKDATA_H

#include "common.h"


class PalettedBlockData {
public:
    PalettedBlockData();

    void setPalette(const vector<BlockID>& newPalette);

    BlockID get(int blockIndex) const;

    void set(int blockIndex, BlockID newBlock);

private:
    vector<BlockID> palette{0};
    vector<uint64_t> data{};

    uint8_t bitsPerEntry{};

    uint64_t entryMask() const { return (1ULL << bitsPerEntry) - 1; }

    void setBitsPerEntry(uint8_t newSize);

    static int getLocalIndex(const vector<BlockID>& palette, BlockID block);

    static size_t calculateDataSize(uint8_t bitsPerEntry);

    static uint8_t calculateBitsPerEntry(size_t s);

    static constexpr int maxBits = 8 * sizeof(BlockID);

    static constexpr int maxBlocks = CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z;
};


#endif //BLOCKGAME_PALETTEDBLOCKDATA_H
