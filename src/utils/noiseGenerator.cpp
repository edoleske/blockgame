#include "noiseGenerator.h"

NoiseGenerator::NoiseGenerator(unsigned int seed): generator(seed) {
    std::iota(permutations.begin(), permutations.end(), uint8_t{ 0 });
    std::shuffle(permutations.begin(), permutations.end(), generator);
}

float NoiseGenerator::get(float x, float y, float z) const {
    const float _x = std::floor(x);
    const float _y = std::floor(y);
    const float _z = std::floor(z);

    const int32_t ix = static_cast<int32_t>(_x) & 255;
    const int32_t iy = static_cast<int32_t>(_y) & 255;
    const int32_t iz = static_cast<int32_t>(_z) & 255;

    const float fx = (x - _x);
    const float fy = (y - _y);
    const float fz = (z - _z);

    const float u = fade(fx);
    const float v = fade(fy);
    const float w = fade(fz);

    const uint8_t A = (permutations[ix & 255] + iy) & 255;
    const uint8_t B = (permutations[(ix + 1) & 255] + iy) & 255;

    const uint8_t AA = (permutations[A] + iz) & 255;
    const uint8_t AB = (permutations[(A + 1) & 255] + iz) & 255;

    const uint8_t BA = (permutations[B] + iz) & 255;
    const uint8_t BB = (permutations[(B + 1) & 255] + iz) & 255;

    const float p0 = grad(permutations[AA], fx, fy, fz);
    const float p1 = grad(permutations[BA], fx - 1, fy, fz);
    const float p2 = grad(permutations[AB], fx, fy - 1, fz);
    const float p3 = grad(permutations[BB], fx - 1, fy - 1, fz);
    const float p4 = grad(permutations[(AA + 1) & 255], fx, fy, fz - 1);
    const float p5 = grad(permutations[(BA + 1) & 255], fx - 1, fy, fz - 1);
    const float p6 = grad(permutations[(AB + 1) & 255], fx, fy - 1, fz - 1);
    const float p7 = grad(permutations[(BB + 1) & 255], fx - 1, fy - 1, fz - 1);

    const float q0 = lerp(u, p0, p1);
    const float q1 = lerp(u, p2, p3);
    const float q2 = lerp(u, p4, p5);
    const float q3 = lerp(u, p6, p7);

    const float r0 = lerp(v, q0, q1);
    const float r1 = lerp(v, q2, q3);

    return lerp(w, r0, r1) * 0.5f + 0.5f;
}

inline constexpr float NoiseGenerator::fade(float t) const {
    return t * t * t * ((6 * t - 15) * t + 10);
}

inline constexpr float NoiseGenerator::lerp(float t, float a, float b) const {
    return t * (b - a) + a;
}

inline constexpr float NoiseGenerator::grad(uint8_t hash, float x, float y, float z) const {
    switch (hash & 0xF) {
        case 0x0: return  x + y;
        case 0x1: return -x + y;
        case 0x2: return  x - y;
        case 0x3: return -x - y;
        case 0x4: return  x + z;
        case 0x5: return -x + z;
        case 0x6: return  x - z;
        case 0x7: return -x - z;
        case 0x8: return  y + z;
        case 0x9: return -y + z;
        case 0xA: return  y - z;
        case 0xB: return -y - z;
        case 0xC: return  y + x;
        case 0xD: return -y + z;
        case 0xE: return  y - x;
        case 0xF: return -y - z;
        default: return 0;
    }
}
