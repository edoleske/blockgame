#include "blockDictionary.h"

#include "block.h"

BlockDictionary* BlockDictionary::_instance = nullptr;

BlockDictionary::BlockDictionary() {
    _instance = this;

    blockTypes[0] = BlockType();
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
}

BlockType BlockDictionary::get(const BlockID id) {
    return blockTypes.find(id)->second;
}

BlockID BlockDictionary::lookup(const string& name) const {
    return blockDictionary.find(name)->second;
}

size_t BlockDictionary::count() const {
    return blockDictionary.size();
}
