#pragma once
#include <glm/glm.hpp>

const float YAW = -90.0f;
const float PITCH = 0.0f;


class Camera{

    public:
        Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
        Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up);
        glm::mat4 getViewMatrix();
        void setPosition(const glm::vec3& position);
        void setTarget(const glm::vec3& target);
        glm::vec3 getPosition() const;
    
    private:
        glm::vec3 position;
        glm::vec3 target;
        glm::vec3 front;
        glm::vec3 up;
        glm::vec3 right;
        glm::vec3 worldUp;
        
};