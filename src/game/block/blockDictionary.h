#ifndef BLOCKGAME_BLOCKDICTIONARY_H
#define BLOCKGAME_BLOCKDICTIONARY_H

#include "common.h"
#include "blockType.h"


class BlockDictionary {
public:
    BlockDictionary();

    ~BlockDictionary();

    static BlockDictionary* getInstance();

    void insert(const BlockType& type);

    BlockType get(BlockID id);

    BlockID lookup(const string& name) const;

    BlockID fromHash(uint16_t hash) const;

    size_t count() const;

private:
    vector<BlockType> blockTypes{};
    unordered_map<string, BlockID> blockDictionary{};
    unordered_map<uint16_t, BlockID> hashDictionary{};

    static BlockDictionary* _instance;
};


#endif //BLOCKGAME_BLOCKDICTIONARY_H
