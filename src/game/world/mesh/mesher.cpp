#include "mesher.h"

Mesher::Mesher(const int vertexCount, const int transparentVertexCount) {
    // This saves a few container expansions for simple rebuilds
    vertices.reserve(vertexCount);
    transparentVertices.reserve(transparentVertexCount);
}

const vector<Vertex>& Mesher::getVertices() const { return vertices; }

const vector<Vertex>& Mesher::getTransparentVertices() const {
    return transparentVertices;
}

void Mesher::addFace(const Block* block, const BlockFace face, const u8vec3& position) {
    for (const auto& vertex : Block::blockFaceVertices[face]) {
        auto v = Vertex(vertex.position + position, vertex.uv, block->getLayer(face));

        if (block->opaque) {
            vertices.push_back(v);
        } else {
            transparentVertices.push_back(v);
        }
    }
}

void Mesher::addFace(const Block* block, const BlockFace face, const u8vec3& origin, const u8vec3& end) {
    if (origin == end) {
        addFace(block, face, origin);
        return;
    }

    auto difference = end - origin;

    for (const auto& vertex : Block::blockFaceVertices[face]) {
        auto uv = vertex.uv;
        switch (face) {
        case BlockFace::FRONT:
        case BlockFace::BACK:
            uv += uv * u8vec2(difference.x, difference.y);
            break;
        case BlockFace::LEFT:
        case BlockFace::RIGHT:
            uv += uv * u8vec2(difference.z, difference.y);
            break;
        case BlockFace::TOP:
        case BlockFace::BOTTOM:
            uv += uv * u8vec2(difference.x, difference.z);
            break;
        }

        auto v = Vertex((vertex.position * difference + vertex.position) + origin, uv, block->getLayer(face));

        if (block->opaque) {
            vertices.push_back(v);
        } else {
            transparentVertices.push_back(v);
        }
    }
}

void Mesher::addBillboard(const Block* block, const u8vec3& position) {
    for (const auto& vertex : Block::billboardVertices) {
        auto v = Vertex(vertex.position + position, vertex.uv, block->getLayer(BlockFace::FRONT));

        if (block->opaque) {
            vertices.push_back(v);
        } else {
            transparentVertices.push_back(v);
        }
    }
}
