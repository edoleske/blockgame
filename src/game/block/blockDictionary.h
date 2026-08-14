#ifndef BLOCKGAME_BLOCKDICTIONARY_H
#define BLOCKGAME_BLOCKDICTIONARY_H

#include "common.h"
#include "block.h"


class BlockDictionary {
public:
    BlockDictionary();

    ~BlockDictionary();

    static BlockDictionary* getInstance();

    void insert(const Block& b);

    Block get(BlockID id);

    BlockID lookup(const string& name) const;

    BlockID fromHash(uint16_t hash) const;

    size_t count() const;

private:
    vector<Block> blocks{};
    unordered_map<string, BlockID> blockDictionary{};
    unordered_map<uint16_t, BlockID> hashDictionary{};

    static BlockDictionary* _instance;
};


#endif //BLOCKGAME_BLOCKDICTIONARY_H
