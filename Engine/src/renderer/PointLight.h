#pragma once
#include <glm/glm.hpp>

struct PointLight {

    PointLight(float x = 0.0f, float y = 0.0f, float z = 0.0f)
        : position(x, y, z) {}

    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 ambient = glm::vec3(0.3f, 0.3f, 0.3f);
    glm::vec3 diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
    glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
};