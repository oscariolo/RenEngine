#pragma once
#include <glm/glm.hpp>

struct Vertex{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 color;
    glm::vec2 texCoords = glm::vec2(0.0f, 0.0f);

    Vertex(const glm::vec3& pos, const glm::vec3& norm, const glm::vec3& col);
    Vertex(float x, float y, float z);

};
