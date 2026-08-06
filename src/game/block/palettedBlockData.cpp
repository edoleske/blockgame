#include "palettedBlockData.h"

PalettedBlockData::PalettedBlockData() {}

void PalettedBlockData::setPalette(const vector<BlockID>& newPalette) {
    auto newBitsPerEntry = calculateBitsPerEntry(newPalette.size());
    if (newBitsPerEntry != bitsPerEntry) {
        setBitsPerEntry(newBitsPerEntry);
    }

    // if data array is compatible between palettes, don't repack
    if (palette.size() <= newPalette.size() && std::equal(palette.begin(), palette.end(), newPalette.begin())) {
        palette = newPalette;
        return;
    }

    vector<uint64_t> newData(data.size());
    for (int w = 0; w < newData.size(); ++w) {
        for (int i = 0; i < 64; i += bitsPerEntry) {
            auto current = (data[w] >> i) & entryMask();
            auto value = getLocalIndex(newPalette, static_cast<int>(current));
            newData[w] |= value << i;
        }
    }
    newData = data;
}

BlockID PalettedBlockData::get(const int blockIndex) const {
    if (bitsPerEntry < 1) return palette[0];

    auto blocksPerRow = 64 / bitsPerEntry;
    auto word = data[blockIndex / blocksPerRow];
    auto shifted = word >> (blockIndex % blocksPerRow * bitsPerEntry);
    auto masked = shifted & entryMask();
    return palette[masked];
}

void PalettedBlockData::set(const int blockIndex, const BlockID newBlock) {
    auto value = getLocalIndex(palette, newBlock);
    if (value >= palette.size()) {
        auto newPalette = palette;
        newPalette.push_back(newBlock);
        setPalette(newPalette);
    }

    auto blocksPerRow = 64 / bitsPerEntry;
    auto word = data[blockIndex / blocksPerRow];
    auto offset = blockIndex % blocksPerRow * bitsPerEntry;
    data[blockIndex / blocksPerRow] = (word & ~(entryMask() << offset)) | ((value & entryMask()) << offset);
}

int PalettedBlockData::getLocalIndex(const vector<BlockID>& palette, const BlockID block) {
    auto it = std::find(palette.begin(), palette.end(), block);
    return static_cast<int>(std::distance(palette.begin(), it));
}

void PalettedBlockData::setBitsPerEntry(const uint8_t newSize) {
    vector<uint64_t> newData(newSize > 0 ? calculateDataSize(newSize) : 0);

    if (bitsPerEntry > 0 && newSize > 0) {
        auto blocksPerRow = 64 / bitsPerEntry;
        auto newIndex = 0;
        auto newPos = 0;
        for (int w = 0; w < data.size(); ++w) {
            for (int i = 0; i < blocksPerRow; ++i) {
                if (w * blocksPerRow + i > maxBlocks) break;

                uint64_t entry = (data.at(w) >> (i % blocksPerRow * bitsPerEntry)) & entryMask();
                newData.at(newIndex) |= entry << newPos;

                newPos += newSize;
                if (newPos + newSize > 64) {
                    newPos = 0;
                    newIndex++;
                }
            }
        }
    }

    data = newData;
    bitsPerEntry = newSize;
}

size_t PalettedBlockData::calculateDataSize(const uint8_t bitsPerEntry) {
    auto blocksPerRow = 64 / bitsPerEntry;
    return (maxBlocks + blocksPerRow - 1) / blocksPerRow;
}

uint8_t PalettedBlockData::calculateBitsPerEntry(const size_t s) {
    if (s <= 1) return 0;
    return std::min(std::bit_width(s - 1), maxBits);
}
