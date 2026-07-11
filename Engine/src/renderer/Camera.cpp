#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : position(position), worldUp(up) {
    // Calculate initial front based on yaw and pitch
    glm::vec3 initialFront;
    initialFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    initialFront.y = sin(glm::radians(pitch));
    initialFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    
    this->front = glm::normalize(initialFront);
    this->target = this->position + this->front;
    this->right = glm::normalize(glm::cross(this->front, worldUp));
    this->up = glm::normalize(glm::cross(this->right, this->front));
}

Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up)
    : position(position), target(target), worldUp(up) {
    this->front = glm::normalize(target - position);
    this->right = glm::normalize(glm::cross(this->front, worldUp));
    this->up = glm::normalize(glm::cross(this->right, this->front));
}

glm::mat4 Camera::getViewMatrix() {
    // Uses the correctly synced up vector
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

void Camera::rotate(float degX, float degY, float degZ) {
    // Calculate new front vector based on rotation angles (degY = yaw, degX = pitch)
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(degY)) * cos(glm::radians(degX));
    newFront.y = sin(glm::radians(degX));
    newFront.z = sin(glm::radians(degY)) * cos(glm::radians(degX));
    
    this->front = glm::normalize(newFront);
    this->target = this->position + this->front;

    this->right = glm::normalize(glm::cross(this->front, worldUp));
    this->up = glm::normalize(glm::cross(this->right, this->front));
}

glm::vec3 Camera::getPosition() const {
    return position;
}