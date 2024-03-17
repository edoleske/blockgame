#include "world.h"

World::World() {
    // Initialize element buffer object shared between all chunks
    ebo = make_shared<ElementBuffer>();
    initializeEBO();

    std::random_device rd;
    seed = rd();
    noise = make_unique<NoiseGenerator>(seed);

    blockTexture = make_shared<BlockTexture>();
    blockTexture->getTexture()->bind();

    // Create save folder
    const auto savePath = "saves/" + name;
    if (!fs::is_directory(savePath) || !fs::exists(savePath)) {
        fs::create_directories(savePath);
    }
}

Chunk* World::getChunk(int x, int z) const {
    auto chunk = chunkMap.find(make_pair(x, z));
    if (chunk != chunkMap.end()) {
        return chunk->second.get();
    }
    return nullptr;
}

bool World::chunkExists(int x, int z) const {
    return chunkMap.find(make_pair(x, z)) != chunkMap.end();
}

Block* World::getBlock(int x, int y, int z) {
    auto chunk = getChunk(x / CHUNK_SIZE_X, z / CHUNK_SIZE_Z);
    if (chunk != nullptr) {
        return chunk->getBlock(x % CHUNK_SIZE_X, y, z % CHUNK_SIZE_Z);
    }
    return nullptr;
}

void World::generateSpawnArea() {
    // Create chunks
    for (int x = -SPAWN_SIZE; x <= SPAWN_SIZE; ++x) {
        for (int z = -SPAWN_SIZE; z <= SPAWN_SIZE; ++z) {
            chunkMap.emplace(make_pair(x, z), make_unique<Chunk>(x, z, ebo, blockTexture));

            auto chunk = chunkMap.at(make_pair(x, z)).get();
            loadFromRegionFile(x, z);
            if (chunk->getChunkState() == ChunkState::EMPTY) {
                chunk->generate(noise.get());
                updateRegionFile(x, z);
            }
        }
    }

    // Build chunk meshes
    for (int x = -SPAWN_SIZE; x <= SPAWN_SIZE; ++x) {
        for (int z = -SPAWN_SIZE; z <= SPAWN_SIZE; ++z) {
            auto chunk = chunkMap.at(make_pair(x, z)).get();
            chunk->buildMesh(chunkMap);
        }
    }
}

void World::loadChunk(int x, int z) {
    chunkMap.emplace(make_pair(x, z), make_unique<Chunk>(x, z, ebo, blockTexture));
    auto chunk = chunkMap.at(make_pair(x, z)).get();
    chunk->generate(noise.get());
    chunk->buildMesh(chunkMap);

    // Rebuild surrounding meshes
    auto leftChunk = getChunk(x - 1, z);
    if (leftChunk != nullptr) {
        leftChunk->buildMesh(chunkMap);
    }

    auto rightChunk = getChunk(x + 1, z);
    if (rightChunk != nullptr) {
        rightChunk->buildMesh(chunkMap);
    }

    auto backChunk = getChunk(x, z - 1);
    if (backChunk != nullptr) {
        backChunk->buildMesh(chunkMap);
    }

    auto frontChunk = getChunk(x, z + 1);
    if (frontChunk != nullptr) {
        frontChunk->buildMesh(chunkMap);
    }
}

void World::unloadChunk(int x, int z) {
    auto it = chunkMap.find(make_pair(x, z));
    if (it != chunkMap.end()) {
        chunkMap.erase(it);
    }

    // Rebuild surrounding meshes
    auto leftChunk = getChunk(x - 1, z);
    if (leftChunk != nullptr) {
        leftChunk->buildMesh(chunkMap);
    }

    auto rightChunk = getChunk(x + 1, z);
    if (rightChunk != nullptr) {
        rightChunk->buildMesh(chunkMap);
    }

    auto backChunk = getChunk(x, z - 1);
    if (backChunk != nullptr) {
        backChunk->buildMesh(chunkMap);
    }

    auto frontChunk = getChunk(x, z + 1);
    if (frontChunk != nullptr) {
        frontChunk->buildMesh(chunkMap);
    }
}

void World::renderWorld(Shader* shader) {
    shader->use();

    for (auto const& chunkRecord: chunkMap) {
        auto chunk = chunkRecord.second.get();
        shader->setInteger("chunkX", chunk->getChunkPosition().x);
        shader->setInteger("chunkZ", chunk->getChunkPosition().z);

        chunk->render();
    }
}

void World::initializeEBO() {
    // There are 2 triangles per face, so 6 indices per face
    auto faceCount = CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z * 6;
    auto data = new GLuint[faceCount * 6];

    int offset = 0;
    for (size_t i = 0; i < faceCount * 6; i += 6) {
        data[i] = 0 + offset;
        data[i + 1] = 1 + offset;
        data[i + 2] = 2 + offset;
        data[i + 3] = 2 + offset;
        data[i + 4] = 3 + offset;
        data[i + 5] = 0 + offset;

        offset += 4;
    }

    ebo->bufferData(faceCount * 6 * sizeof(GLuint), data);
}

string World::getRegionFilePath(int x, int z) const {
    return "saves/" + name + "/" +
           "region." + std::to_string(x >> 5) + "." + std::to_string(z >> 5) + ".data";
}

void World::createRegionFile(const string& filepath) {
    if (fs::exists(filepath)) {
        return;
    }

    const int sectorCount = 32 * 32 * 48 + 2;
    vector<char> fileData(sectorCount * REGION_SECTOR_SIZE);

    // Populate header
    int offset = 2;
    const uint8_t chunkSectors = 48;
    const char chunkSectorsByte = static_cast<char>(chunkSectors);

    for (int i = 0; i < REGION_SECTOR_SIZE; i += 4) {
        fileData[i] = static_cast<char>(offset);
        fileData[i + 1] = static_cast<char>(offset >> 8);
        fileData[i + 2] = static_cast<char>(offset >> 16);
        fileData[i + 3] = chunkSectorsByte;

        offset += chunkSectors;
    }

    ofstream of;
    of.open(filepath, std::ios::binary);
    if (of.is_open()) {
        of.write(fileData.data(), fileData.size());
    }
    of.close();
}

void World::updateRegionFile(int x, int z) const {
    auto chunk = getChunk(x, z);
    if (chunk == nullptr) {
        return;
    }

    auto byteCount = CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z * 3;
    byteCount += 4096 - (byteCount % 4096);
    vector<char> chunkData(byteCount);

    const int headerOffset = 4 * ((x & 31) + (z & 31) * 32);
    chunk->write(chunkData);

    // Create shell for region file if it doesn't exist
    auto regionPath = getRegionFilePath(x, z);
    if (!fs::exists(regionPath)) {
        createRegionFile(regionPath);
    }

    fstream fs;
    fs.open(regionPath, std::ios::binary | std::ios::in | std::ios::out);
    if (!fs.is_open()) {
        std::cerr << "Unable to open file " << getRegionFilePath(x, z) << std::endl;
        return;
    }

    fs.seekg(headerOffset);
    uint8_t byte0 = fs.get();
    uint8_t byte1 = fs.get();
    uint8_t byte2 = fs.get();
    int chunkOffset = byte0 | (byte1 << 8) | (byte2 << 16);

    fs.seekg(headerOffset + REGION_SECTOR_SIZE);
    unsigned epochTime = std::time(nullptr);
    fs.write(reinterpret_cast<const char*>(&epochTime), sizeof(epochTime));

    fs.seekg(chunkOffset * REGION_SECTOR_SIZE);
    fs.write(chunkData.data(), chunkData.size());
    fs.close();
}

void World::loadFromRegionFile(int x, int z) {
    auto chunk = getChunk(x, z);
    if (chunk == nullptr) {
        return;
    }

    const int headerOffset = 4 * ((x & 31) + (z & 31) * 32);

    ifstream ifs;
    ifs.open(getRegionFilePath(x, z), std::ios::binary);
    if (!ifs.is_open()) {
        std::cerr << "Unable to open file " << getRegionFilePath(x, z) << std::endl;
        return;
    }

    ifs.seekg(headerOffset);
    uint8_t byte0 = ifs.get();
    uint8_t byte1 = ifs.get();
    uint8_t byte2 = ifs.get();
    int chunkOffset = byte0 | (byte1 << 8) | (byte2 << 16);

    // Check if chunk data is missing
    ifs.seekg(headerOffset + REGION_SECTOR_SIZE);
    unsigned epochTime;
    ifs.read((char*) &epochTime, sizeof(epochTime));
    if (epochTime == 0) {
        return;
    }

    ifs.seekg(chunkOffset * REGION_SECTOR_SIZE);
    chunk->load(ifs);
    ifs.close();
}
