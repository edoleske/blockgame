#include "blockDictionary.h"

#include "block.h"

BlockDictionary* BlockDictionary::_instance = nullptr;

BlockDictionary::BlockDictionary() {
    _instance = this;
    blockTypes.resize(MAX_BLOCK_TYPES);
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

void BlockDictionary::insert(const BlockType& type) {
    blockTypes[type.id] = type;
    blockDictionary[type.name] = type.id;

    std::hash<BlockType> blockHasher;
    hashDictionary[blockHasher(type)] = type.id;
}

BlockType BlockDictionary::get(const BlockID id) {
    return blockTypes[id];
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
