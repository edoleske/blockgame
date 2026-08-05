#include "palettedBlockData.h"

PalettedBlockData::PalettedBlockData() {}

void PalettedBlockData::setPalette(const vector<BlockID>& newPalette) {
    auto newBitsPerEntry = calculateBitsPerEntry(newPalette.size());
    vector<uint64_t> newData(calculateDataSize(newPalette.size(), newBitsPerEntry));


}

size_t PalettedBlockData::calculateDataSize(const size_t blockCount, const uint8_t bitsPerEntry) {
    auto blocksPerRow = 64 / bitsPerEntry;
    return (blockCount + blocksPerRow - 1) / blocksPerRow;
}

uint8_t PalettedBlockData::calculateBitsPerEntry(const size_t s) {
    if (s <= 1) return 0;
    return std::min(std::bit_width(s - 1), static_cast<int>(8 * sizeof(BlockID)));
}
