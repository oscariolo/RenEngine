#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Projections {
    public:
        static glm::mat4 perspective(float aspect=1.0f) {
            return glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
        }

};