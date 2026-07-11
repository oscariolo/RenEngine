#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : position(position), target(position + glm::vec3(0.0f, 0.0f, -1.0f)), worldUp(up), front(glm::vec3(0.0f, 0.0f, -1.0f)) {
    // Calculate the initial right and up vectors
    this->right = glm::normalize(glm::cross(front, worldUp));
    this->up = glm::normalize(glm::cross(this->right, front));
}

Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up)
    : position(position), target(target), worldUp(up) {
    this->front = glm::normalize(target - position);
    this->right = glm::normalize(glm::cross(this->front, worldUp));
    this->up = glm::normalize(glm::cross(this->right, this->front));
}

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(position, target, up);
}

void Camera::setPosition(const glm::vec3& position) {
    this->position = position;
    this->front = glm::normalize(target - this->position);
    this->right = glm::normalize(glm::cross(this->front, worldUp));
    this->up = glm::normalize(glm::cross(this->right, this->front));
}

void Camera::setTarget(const glm::vec3& target) {
    this->target = target;
    this->front = glm::normalize(this->target - this->position);
    this->right = glm::normalize(glm::cross(this->front, worldUp));
    this->up = glm::normalize(glm::cross(this->right, this->front));
}

glm::vec3 Camera::getPosition() const {
    return position;
}