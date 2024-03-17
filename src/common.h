#ifndef BLOCKGAME_COMMON_H
#define BLOCKGAME_COMMON_H

// C/C++ library headers
#include <iostream>
#include <memory>
#include <array>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>
#include <string>
#include <sstream>
#include <fstream>
#include <random>
#include <algorithm>
#include <exception>
#include <filesystem>
#include <utility>

using std::string;
using std::stringstream;
using std::fstream;
using std::ifstream;
using std::ofstream;
using std::unique_ptr;
using std::make_unique;
using std::shared_ptr;
using std::make_shared;
using std::map;
using std::unordered_map;
using std::array;
using std::vector;
using std::set;
using std::pair;
using std::make_pair;

namespace fs = std::filesystem;

// GLM for vector/matrix math
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::ivec3;
using glm::u8vec3;
using glm::mat4;

// Ensure GLAD is included before GLFW
#include "glad/gl.h"
#include <GLFW/glfw3.h>

// Global constants
const int CHUNK_SIZE_X = 16;
const int CHUNK_SIZE_Y = 254;
const int CHUNK_SIZE_Z = 16;

const int REGION_SECTOR_SIZE = 4096;
constexpr int RENDER_DISTANCE = 4;

struct IntPairHash {
            std::size_t operator() (const pair<int, int> &v) const {
                return std::hash<int>()(v.first) ^ std::hash<int>()(v.second) << 1;
            }
};

#endif //BLOCKGAME_COMMON_H
