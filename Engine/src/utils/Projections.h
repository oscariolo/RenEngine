#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Projections {
    public:
        static glm::mat4 perspective(float fov = 45.0f, float aspectRatio = 1.0f, float nearPlane = 0.1f, float farPlane = 100.0f){
            return glm::perspective(fov, aspectRatio, nearPlane, farPlane);
        }

};