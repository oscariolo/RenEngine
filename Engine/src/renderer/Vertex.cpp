#include "Vertex.h"

Vertex::Vertex(const glm::vec3& pos, const glm::vec3& norm, const glm::vec3& col)
    : position(pos), normal(norm), color(col) {}

Vertex::Vertex(float x, float y, float z){
    position = glm::vec3(x, y, z);
    normal = glm::vec3(0.0f, 0.0f, 0.0f); // Default normal
    color = glm::vec3(1.0f, 1.0f, 1.0f); // Default color (white)
}