#include "palettedBlockData.h"

#include "blockDictionary.h"
#include "block.h"

PalettedBlockData::PalettedBlockData() = default;

PalettedBlockData::PalettedBlockData(const vector<char>& buffer) {
    vector<uint16_t> hashes;
    uint32_t paletteSize, dataSize;
    auto index = 0;

    std::memcpy(&paletteSize, buffer.data() + index, sizeof(paletteSize));
    index += sizeof(paletteSize);
    hashes.resize(paletteSize);
    palette.resize(paletteSize);
    std::memcpy(&dataSize, buffer.data() + index, sizeof(dataSize));
    index += sizeof(dataSize);
    data.resize(dataSize);

    std::memcpy(hashes.data(), buffer.data() + index, paletteSize * sizeof(uint16_t));
    index += paletteSize * sizeof(uint16_t);

    // Parse palette using block dictionary
    auto dict = BlockDictionary::getInstance();
    for (auto i = 0; i < hashes.size(); ++i) {
        palette[i] = dict->fromHash(hashes[i]);
    }

    std::memcpy(data.data(), buffer.data() + index, dataSize * sizeof(uint64_t));
    bitsPerEntry = calculateBitsPerEntry(palette.size());
}

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

void PalettedBlockData::write(vector<char>& byteData) const {
    auto paletteSize = static_cast<uint32_t>(palette.size());
    auto dataSize = static_cast<uint32_t>(data.size());

    // Resize input vector to allow appending data to end
    auto size = byteData.size();
    if (size < this->size()) {
        byteData.resize(this->size());
    }

    // Transform palette from ids to hashes
    vector<uint16_t> hashes(paletteSize);
    auto dict = BlockDictionary::getInstance();
    for (int i = 0; i < paletteSize; ++i) {
        hashes[i] = std::hash<Block>()(dict->get(palette[i]));
    }

    auto index = byteData.data();
    std::memcpy(index, &paletteSize, sizeof(paletteSize));
    index += sizeof(paletteSize);
    std::memcpy(index , &dataSize, sizeof(dataSize));
    index += sizeof(dataSize);
    std::memcpy(index, hashes.data(), paletteSize * sizeof(uint16_t));
    index += paletteSize * sizeof(uint16_t);
    std::memcpy(index, data.data(), dataSize * sizeof(uint64_t));
}

unsigned long PalettedBlockData::size() const {
    return 2 * sizeof(uint32_t) + palette.size() * sizeof(uint16_t) + data.size() * sizeof(uint64_t);
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
