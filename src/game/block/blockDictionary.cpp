#include "blockDictionary.h"

#include "block.h"

BlockDictionary* BlockDictionary::_instance = nullptr;

BlockDictionary::BlockDictionary() {
    _instance = this;
    blocks.resize(MAX_BLOCK_TYPES);
    hashDictionary[0] = 0;
}

BlockDictionary::~BlockDictionary() {
    if (_instance != nullptr) {
        _instance = nullptr;
    }
}

BlockDictionary* BlockDictionary::getInstance() {
    return _instance;
}

void BlockDictionary::insert(const Block& b) {
    blocks[b.id] = b;
    blockDictionary[b.name] = b.id;

    std::hash<Block> blockHasher;
    hashDictionary[blockHasher(b)] = b.id;
}

const Block* BlockDictionary::get(const BlockID id) const {
    return &blocks[id];
}

BlockID BlockDictionary::lookup(const string& name) const {
    return blockDictionary.find(name)->second;
}

BlockID BlockDictionary::fromHash(const uint16_t hash) const {
    return hashDictionary.find(hash)->second;
}

size_t BlockDictionary::count() const {
    return blockDictionary.size();
}
