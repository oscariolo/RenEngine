#include "Mesh.h"
#include <glad/glad.h>

Mesh::Mesh() {
    // Constructor implementation (if needed)
}

Mesh::~Mesh() {
    // Destructor implementation (if needed)
}

void Mesh::setupBuffers() {
    // Generate and bind VAO, VBO, EBO
    glGenVertexArrays(1, &VAO_ID);
    glGenBuffers(1, &VBO_ID);
    glGenBuffers(1, &EBO_ID);

    glBindVertexArray(VAO_ID);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);
    glBufferData(GL_ARRAY_BUFFER, this->getVertices().size() * sizeof(Vertex), this->getVertices().data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->getIndices().size() * sizeof(unsigned int), this->getIndices().data(), GL_STATIC_DRAW);

    // Set vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(2);

    // Unbind VAO
    glBindVertexArray(0);
}

std::vector<Vertex> Mesh::getVertices() const {
    return vertices;
}

std::vector<unsigned int> Mesh::getIndices() const {
    return indices;
}
