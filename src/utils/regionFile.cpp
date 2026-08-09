#include "regionFile.h"

#include <format>

#include "log.h"

RegionFile::RegionFile(const string& worldName, const int x, const int z) {
    path = getRegionFilePath(worldName, x, z);
    LOG_DEBUG("Loading region file {}", path.string());

    // Create if not exists
    if (!fs::exists(path)) {
        ofstream createFile(path);
    }

    file.open(path, fstream::in | fstream::out | fstream::binary);
    if (!file.is_open()) {
        throw std::runtime_error(std::format("Could not open file for writing: {}", path.string()));
    }

    // Write sector for location lookup table if missing
    updateSectorCount();
    if (sectorCount == 0) {
        LOG_DEBUG("Creating location table for {}", path.string());
        vector<char> buffer(SECTOR_SIZE, 0);
        file.write(buffer.data(), buffer.size());
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
    LOG_DEBUG("Closing region file {}", path.string());
    if (file.is_open()) {
        file.close();
    }
}

void RegionFile::write(const int x, const int z, const PalettedBlockData& blockData) {
    uint32_t offset = getOffset(x, z);
    uint32_t sectorOffset = offset >> 8;
    auto sectorsNeeded = (blockData.size() + SECTOR_SIZE - 1) / SECTOR_SIZE;
    LOG_DEBUG("Writing {} sector(s) to {}", sectorsNeeded, path.string());

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
            vector<char> buffer(SECTOR_SIZE, 0);
            file.write(buffer.data(), buffer.size());
            sectorFreeMap.insert(sectorFreeMap.end(), sectorsNeeded, false);
            file.clear();
        }

        // Update location value
        auto pos = ((x & 31) + (z & 31) * 32) * 4;
        file.seekg(pos);
        uint32_t newOffset = sectorOffset << 8;
        newOffset |= sectorsNeeded & 0xFF;
        file.write(reinterpret_cast<const char*>(&newOffset), sizeof(uint32_t));
    }

    vector<char> data(sectorsNeeded * SECTOR_SIZE, 0);
    blockData.write(data);

    file.seekg(sectorOffset * SECTOR_SIZE);
    file.write(data.data(), data.size());

    file.flush();
    updateSectorCount();
}

optional<PalettedBlockData> RegionFile::load(const int x, const int z) {
    auto offset = getOffset(x, z);
    if (offset >> 8 == 0 || (offset >> 8) + (offset & 0xFF) > sectorCount) {
        return std::nullopt;
    }

    LOG_DEBUG("Loading {} sectors to chunk {} {}", offset & 0xFF, x, z);
    vector<char> buffer((offset & 0xFF) * SECTOR_SIZE);
    file.seekg((offset >> 8) * SECTOR_SIZE);
    file.read(buffer.data(), (offset & 0xFF) * SECTOR_SIZE);
    return PalettedBlockData(buffer);
}

uint32_t RegionFile::getOffset(const int x, const int z) {
    auto pos = ((x & 31) + (z & 31) * 32) * 4;
    file.seekg(pos);
    uint32_t offset;
    file.read(reinterpret_cast<char*>(&offset), sizeof(uint32_t));
    return offset;
}

void RegionFile::updateSectorCount() {
    auto size = fs::file_size(path);
    sectorCount = size / SECTOR_SIZE;
}

string RegionFile::getRegionFilePath(const string& worldName, const int x, const int z) {
    auto filename = std::format("region.{}.{}.data", x >> 5, z >> 5);
    return fs::current_path() / "saves" / worldName / filename;
}
