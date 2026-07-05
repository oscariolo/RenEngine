#include "Cube.h"

Cube::Cube() {
    generateVertices();
    setupBuffers();
}

Cube::~Cube() {
    // Destructor implementation (if needed)
}

void Cube::generateVertices() {

        // Define the vertices of the cube
    vertices = {
        Vertex(-0.5f, -0.5f, -0.5f), // 0
            Vertex( 0.5f, -0.5f, -0.5f), // 1
            Vertex( 0.5f,  0.5f, -0.5f), // 2
            Vertex(-0.5f,  0.5f, -0.5f), // 3
            Vertex(-0.5f, -0.5f,  0.5f), // 4
            Vertex( 0.5f, -0.5f,  0.5f), // 5
            Vertex( 0.5f,  0.5f,  0.5f), // 6
            Vertex(-0.5f,  0.5f,  0.5f)  // 7
        };

        // Define the indices for the cube's faces (two triangles per face)
        indices = {
            // Back face
            0, 1, 2,
            2, 3, 0,
            // Front face
            4, 5, 6,
            6, 7, 4,
            // Left face
            4, 7, 3,
            3, 0, 4,
            // Right face
            1, 5, 6,
            6, 2, 1,
            // Bottom face
            4, 1, 0,
            1, 4, 5,
            // Top face
            3, 2, 6,
            6, 7, 3
        };
}
