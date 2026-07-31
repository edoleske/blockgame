#include "blockDictionary.h"

#include "block.h"

BlockDictionary* BlockDictionary::_instance = nullptr;

BlockDictionary::BlockDictionary() {
    _instance = this;
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

BlockType BlockDictionary::get(const BlockID id) const {
    return blockTypes[id];
}

BlockID BlockDictionary::lookup(const string& name) {
    if (!blockDictionary.contains(name)) return 0;
    return blockDictionary[name];
}
