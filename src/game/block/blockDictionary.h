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

    BlockType get(BlockID id) const;

    BlockID lookup(const string& name);

    size_t count() const;

private:
    unordered_map<BlockID, BlockType> blockTypes{};
    unordered_map<string, BlockID> blockDictionary{};

    static BlockDictionary* _instance;
};


#endif //BLOCKGAME_BLOCKDICTIONARY_H
