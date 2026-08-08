#ifndef BLOCKGAME_REGIONFILE_H
#define BLOCKGAME_REGIONFILE_H

#include "common.h"
#include "game/block/palettedBlockData.h"


class RegionFile {
    public:
    explicit RegionFile(const string& worldName, int x, int z);
    ~RegionFile();

    void write(int x, int z, const PalettedBlockData& blockData);

    optional<PalettedBlockData> load(int x, int z);

    static string getRegionFilePath(const string& worldName, int x, int z);

    static constexpr int SECTOR_SIZE = 4096;

private:
    fs::path path;
    fstream file;

    uintmax_t sectorCount{};
    vector<bool> sectorFreeMap{true};

    uint32_t getOffset(int x, int z);

    void updateSectorCount();
};


#endif //BLOCKGAME_REGIONFILE_H
