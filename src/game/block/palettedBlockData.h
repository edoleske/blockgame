#ifndef BLOCKGAME_PALETTEDBLOCKDATA_H
#define BLOCKGAME_PALETTEDBLOCKDATA_H

#include "common.h"


class PalettedBlockData {
public:
    PalettedBlockData();

    void setPalette(const vector<BlockID>& newPalette);

private:
    vector<BlockID> palette;
    vector<uint64_t> data;

    uint8_t bitsPerEntry;

    static size_t calculateDataSize(size_t blockCount, uint8_t bitsPerEntry);
    static uint8_t calculateBitsPerEntry(size_t s);
};


#endif //BLOCKGAME_PALETTEDBLOCKDATA_H
