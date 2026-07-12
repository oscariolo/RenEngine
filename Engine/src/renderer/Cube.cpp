#include "Cube.h"

Cube::Cube() {
    generateVertices();
    setupBuffers();
    setupTexture();
}

Cube::~Cube() {
    // Destructor implementation (if needed)
}

void Cube::generateVertices() {
    // Default color for the cube (white)
    glm::vec3 col(0.5f, 0.5f, 0.5f);

    // Define 24 vertices (4 for each of the 6 faces) to ensure flat, sharp normals
    vertices = {
        // FRONT FACE (Z = 0.5) - Normal points strictly towards +Z
        Vertex(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 1.0f), col), // 0
        Vertex(glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 1.0f), col), // 1
        Vertex(glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 1.0f), col), // 2
        Vertex(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 1.0f), col), // 3

        // BACK FACE (Z = -0.5) - Normal points strictly towards -Z
        Vertex(glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), col), // 4
        Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), col), // 5
        Vertex(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), col), // 6
        Vertex(glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), col), // 7

        // LEFT FACE (X = -0.5) - Normal points strictly towards -X
        Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), col), // 8
        Vertex(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), col), // 9
        Vertex(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), col), // 10
        Vertex(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), col), // 11

        // RIGHT FACE (X = 0.5) - Normal points strictly towards +X
        Vertex(glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(1.0f, 0.0f, 0.0f), col),  // 12
        Vertex(glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f), col),  // 13
        Vertex(glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f), col),  // 14
        Vertex(glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(1.0f, 0.0f, 0.0f), col),  // 15

        // TOP FACE (Y = 0.5) - Normal points strictly towards +Y
        Vertex(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f, 1.0f, 0.0f), col),  // 16
        Vertex(glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(0.0f, 1.0f, 0.0f), col),  // 17
        Vertex(glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), col),  // 18
        Vertex(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), col),  // 19

        // BOTTOM FACE (Y = -0.5) - Normal points strictly towards -Y
        Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), col), // 20
        Vertex(glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), col), // 21
        Vertex(glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f, 0.0f), col), // 22
        Vertex(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f, 0.0f), col)  // 23
    };

    // Every face now uses its own unique batch of 4 vertices
    indices = {
        // Front face
         0,  1,  2,      2,  3,  0,
        // Back face
         4,  5,  6,      6,  7,  4,
        // Left face
         8,  9, 10,     10, 11,  8,
        // Right face
        12, 13, 14,     14, 15, 12,
        // Top face
        16, 17, 18,     18, 19, 16,
        // Bottom face
        20, 21, 22,     22, 23, 20
    };

    // Texture coordinates for each vertex (same for all faces)
    for (int i = 0; i < 6; ++i) {
        vertices[i * 4 + 0].texCoords = glm::vec2(0.0f, 0.0f); // Bottom-left
        vertices[i * 4 + 1].texCoords = glm::vec2(1.0f, 0.0f); // Bottom-right
        vertices[i * 4 + 2].texCoords = glm::vec2(1.0f, 1.0f); // Top-right
        vertices[i * 4 + 3].texCoords = glm::vec2(0.0f, 1.0f); // Top-left
    }

}
