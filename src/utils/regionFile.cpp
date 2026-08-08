#include "regionFile.h"

#include <format>

RegionFile::RegionFile(const string& worldName, const int x, const int z) {
    path = getRegionFilePath(worldName, x, z);
    file.open(path, fstream::in | fstream::out | fstream::binary | fstream::trunc);
    if (!file.is_open()) {
        throw std::runtime_error(std::format("Could not open file for writing: {}", path.string()));
    }

    // Write sector for location lookup table if missing
    updateSectorCount();
    if (sectorCount == 0) {
        vector<char> buffer(SECTOR_SIZE, 0);
        file.write(buffer.data(), SECTOR_SIZE);
        file.flush();
        updateSectorCount();
    }

    // Create map
    sectorFreeMap[0] = false;
    for (int i = 0; i < SECTOR_SIZE; i += sizeof(uint32_t)) {
        file.seekg(i);
        uint32_t offset;
        file.read(reinterpret_cast<char*>(&offset), sizeof(uint32_t));
        if (offset >> 8 == 0 || (offset & 0xFF) > sectorFreeMap.size()) continue;

        for (int s = 0; s < (offset & 0xFF); ++s) {
            sectorFreeMap[(offset << 8) + s] = false;
        }
    }
}

RegionFile::~RegionFile() {
    if (file.is_open()) {
        file.close();
    }
}

void RegionFile::write(const int x, const int z, const PalettedBlockData& blockData) {
    uint32_t offset = getOffset(x, z);
    uint32_t sectorOffset = offset >> 8;
    auto sectorsNeeded = blockData.size() / SECTOR_SIZE;
    if (sectorOffset == 0 || (offset & 0xFF) != sectorsNeeded) {
        sectorOffset = sectorCount;

        auto it = std::find(sectorFreeMap.begin(), sectorFreeMap.end(), true);
        int start = std::distance(sectorFreeMap.begin(), it);
        int length = 0;
        if (it != sectorFreeMap.end()) {
            for (int i = start; i < sectorFreeMap.size(); ++i) {
                if (length != 0) {
                    if (sectorFreeMap[i]) {
                        length++;
                    } else {
                        length = 0;
                    }
                } else if (sectorFreeMap[i]) {
                    start = i;
                    length = 1;
                }
                if (length >= sectorsNeeded) break;
            }
        }

        if (length >= sectorsNeeded) {
            sectorOffset = start;
        } else {
            file.seekg(0, fstream::end);
            char c = 0;
            file.write(&c, sectorsNeeded * SECTOR_SIZE);
            sectorFreeMap.insert(sectorFreeMap.end(), sectorsNeeded, false);
        }

        // Update location value
        file.seekg(((x & 31) + (z & 31) * 32) * 4);
        uint32_t offset = sectorOffset << 8;
        offset |= sectorsNeeded & 0xFF;
        file.write(reinterpret_cast<char*>(&offset), sizeof(uint32_t));
    }

    vector<char> data(sectorsNeeded * SECTOR_SIZE, 0);
    blockData.write(data);

    file.seekg(sectorOffset * SECTOR_SIZE);
    file.write(data.data(), data.size());
}

optional<PalettedBlockData> RegionFile::load(const int x, const int z) {
    auto offset = getOffset(x, z);
    if (offset >> 8 == 0 || (offset & 0xFF) > sectorCount) {
        return std::nullopt;
    }

    vector<char> buffer((offset & 0xFF) * SECTOR_SIZE);
    file.seekg(offset >> 8);
    file.read(buffer.data(), (offset & 0xFF) * SECTOR_SIZE);
    return {buffer};
}

uint32_t RegionFile::getOffset(const int x, const int z) {
    file.seekg(((x & 31) + (z & 31) * 32) * 4);
    uint32_t offset;
    file.read(reinterpret_cast<char*>(&offset), sizeof(uint32_t));
    return offset;
}

void RegionFile::updateSectorCount() {
    sectorCount = fs::file_size(path) / SECTOR_SIZE;
}

string RegionFile::getRegionFilePath(const string& worldName, const int x, const int z) {
    auto filename = std::format("region.{}.{}.data", x >> 5, z >> 5);
    return fs::current_path() / "saves" / worldName / filename;
}
