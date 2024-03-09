#include "block.h"

Block::Block(BlockType type): type(type) {}

void Block::addBlockFaceVertices(vector<Vertex> &vertices, BlockFace face, u8vec3 &chunkPosition) {
    switch (face) {
        case BlockFace::TOP:
            vertices.insert(vertices.end(), {
                    {{0 + chunkPosition.x, 1 + chunkPosition.y, 0 + chunkPosition.z}, {0, 0}},
                    {{1 + chunkPosition.x, 1 + chunkPosition.y, 0 + chunkPosition.z}, {1, 0}},
                    {{1 + chunkPosition.x, 1 + chunkPosition.y, 1 + chunkPosition.z}, {1, 1}},
                    {{1 + chunkPosition.x, 1 + chunkPosition.y, 1 + chunkPosition.z}, {1, 1}},
                    {{0 + chunkPosition.x, 1 + chunkPosition.y, 1 + chunkPosition.z}, {0, 1}},
                    {{0 + chunkPosition.x, 1 + chunkPosition.y, 0 + chunkPosition.z}, {0, 0}},
            });
            break;
        case BlockFace::BOTTOM:
            vertices.insert(vertices.end(), {
                    {{0 + chunkPosition.x, 0 + chunkPosition.y, 0 + chunkPosition.z}, {0, 0}},
                    {{0 + chunkPosition.x, 0 + chunkPosition.y, 1 + chunkPosition.z}, {0, 1}},
                    {{1 + chunkPosition.x, 0 + chunkPosition.y, 1 + chunkPosition.z}, {1, 1}},
                    {{1 + chunkPosition.x, 0 + chunkPosition.y, 1 + chunkPosition.z}, {1, 1}},
                    {{1 + chunkPosition.x, 0 + chunkPosition.y, 0 + chunkPosition.z}, {1, 0}},
                    {{0 + chunkPosition.x, 0 + chunkPosition.y, 0 + chunkPosition.z}, {0, 0}},
            });
            break;
        case BlockFace::LEFT:
            vertices.insert(vertices.end(), {
                    {{0 + chunkPosition.x, 1 + chunkPosition.y, 1 + chunkPosition.z}, {0, 1}},
                    {{0 + chunkPosition.x, 0 + chunkPosition.y, 1 + chunkPosition.z}, {1, 1}},
                    {{0 + chunkPosition.x, 0 + chunkPosition.y, 0 + chunkPosition.z}, {1, 0}},
                    {{0 + chunkPosition.x, 0 + chunkPosition.y, 0 + chunkPosition.z}, {1, 0}},
                    {{0 + chunkPosition.x, 1 + chunkPosition.y, 0 + chunkPosition.z}, {0, 0}},
                    {{0 + chunkPosition.x, 1 + chunkPosition.y, 1 + chunkPosition.z}, {0, 1}},
            });
            break;
        case BlockFace::RIGHT:
            vertices.insert(vertices.end(), {
                    {{1 + chunkPosition.x, 1 + chunkPosition.y, 1 + chunkPosition.z}, {0, 0}},
                    {{1 + chunkPosition.x, 1 + chunkPosition.y, 0 + chunkPosition.z}, {0, 1}},
                    {{1 + chunkPosition.x, 0 + chunkPosition.y, 0 + chunkPosition.z}, {1, 1}},
                    {{1 + chunkPosition.x, 0 + chunkPosition.y, 0 + chunkPosition.z}, {1, 1}},
                    {{1 + chunkPosition.x, 0 + chunkPosition.y, 1 + chunkPosition.z}, {1, 0}},
                    {{1 + chunkPosition.x, 1 + chunkPosition.y, 1 + chunkPosition.z}, {0, 0}},
            });
            break;
        case BlockFace::FRONT:
            vertices.insert(vertices.end(), {
                    {{0 + chunkPosition.x, 0 + chunkPosition.y, 1 + chunkPosition.z}, {0, 1}},
                    {{0 + chunkPosition.x, 1 + chunkPosition.y, 1 + chunkPosition.z}, {0, 0}},
                    {{1 + chunkPosition.x, 1 + chunkPosition.y, 1 + chunkPosition.z}, {1, 0}},
                    {{1 + chunkPosition.x, 1 + chunkPosition.y, 1 + chunkPosition.z}, {1, 0}},
                    {{1 + chunkPosition.x, 0 + chunkPosition.y, 1 + chunkPosition.z}, {1, 1}},
                    {{0 + chunkPosition.x, 0 + chunkPosition.y, 1 + chunkPosition.z}, {0, 1}},
            });
            break;
        case BlockFace::BACK:
            vertices.insert(vertices.end(), {
                    {{0 + chunkPosition.x, 0 + chunkPosition.y, 0 + chunkPosition.z}, {1, 1}},
                    {{1 + chunkPosition.x, 0 + chunkPosition.y, 0 + chunkPosition.z}, {0, 1}},
                    {{1 + chunkPosition.x, 1 + chunkPosition.y, 0 + chunkPosition.z}, {0, 0}},
                    {{1 + chunkPosition.x, 1 + chunkPosition.y, 0 + chunkPosition.z}, {0, 0}},
                    {{0 + chunkPosition.x, 1 + chunkPosition.y, 0 + chunkPosition.z}, {1, 0}},
                    {{0 + chunkPosition.x, 0 + chunkPosition.y, 0 + chunkPosition.z}, {1, 1}},
            });
            break;
    }
}

BlockType Block::getType() const {
    return type;
}
