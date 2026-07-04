#include "Mesh.h"

Mesh::Mesh() {
    // Constructor implementation (if needed)
}

Mesh::~Mesh() {
    // Destructor implementation (if needed)
}

std::vector<Vertex> Mesh::getVertices() const {
    return vertices;
}

std::vector<unsigned int> Mesh::getIndices() const {
    return indices;
}
