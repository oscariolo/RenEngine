#pragma once
#include <Engine.h>


class Brick : public PhysicsObject {
    public:
        using PhysicsObject::PhysicsObject; // Inherit constructors from PhysicsObject

    public:
        bool isBeingDestroyed = false;
        float destructionTimer = 0.0f;
        void killBrick();
        void update()override;


};