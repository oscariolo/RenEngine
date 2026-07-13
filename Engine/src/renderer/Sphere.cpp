#include "Sphere.h"

#include <glm/glm.hpp>
#include <cmath>

namespace {
    constexpr float PI = 3.14159265359f;
}

Sphere::Sphere(unsigned int stacks, unsigned int sectors, float radius) {
    generateVertices(stacks, sectors, radius);
    setupBuffers();
}

Sphere::~Sphere() {
}

void Sphere::generateVertices(unsigned int stacks, unsigned int sectors, float radius) {
    vertices.clear();
    indices.clear();

    for (unsigned int i = 0; i <= stacks; ++i) {
        float stackAngle = PI / 2.0f - static_cast<float>(i) * PI / static_cast<float>(stacks);
        float xy = radius * std::cos(stackAngle);
        float z = radius * std::sin(stackAngle);

        for (unsigned int j = 0; j <= sectors; ++j) {
            float sectorAngle = static_cast<float>(j) * 2.0f * PI / static_cast<float>(sectors);

            float x = xy * std::cos(sectorAngle);
            float y = xy * std::sin(sectorAngle);

            glm::vec3 position(x, y, z);
            glm::vec3 normal = glm::normalize(position);
            glm::vec3 color(0.95f, 0.95f, 1.0f);
            vertices.emplace_back(position, normal, color);
        }
    }

    for (unsigned int i = 0; i < stacks; ++i) {
        unsigned int k1 = i * (sectors + 1);
        unsigned int k2 = k1 + sectors + 1;

        for (unsigned int j = 0; j < sectors; ++j, ++k1, ++k2) {
            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            if (i != (stacks - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }

    //agregar text coord
    for(unsigned int i = 0; i <= stacks; ++i) {
        for(unsigned int j = 0; j <= sectors; ++j) {
            float s = static_cast<float>(j) / static_cast<float>(sectors);
            float t = static_cast<float>(i) / static_cast<float>(stacks);
            vertices[i * (sectors + 1) + j].texCoords = glm::vec2(s, t);
        }
    }

}
