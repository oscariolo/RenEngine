#pragma once
#include <Engine.h>


class Brick : public PhysicsObject {
    public:
        using PhysicsObject::PhysicsObject; // Inherit constructors from PhysicsObject

    public:
        bool isBeingDestroyed = false;
        float destructionTimer = 0.0f;
        glm::vec3 originalScale = glm::vec3(1.0f, 1.0f, 1.0f);
        void killBrick();
        void reset();
        void update()override;
        void setScale(const glm::vec3& newScale) {
            originalScale = newScale;
            scale = newScale;
        }


};