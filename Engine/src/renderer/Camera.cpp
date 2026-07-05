#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : position(position), worldUp(up), front(glm::vec3(0.0f, 0.0f, -1.0f)) {
    // Calculate the initial right and up vectors
    this->right = glm::normalize(glm::cross(front, worldUp));
    this->up = glm::normalize(glm::cross(this->right, front));
}

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(position, position + front, up);
}