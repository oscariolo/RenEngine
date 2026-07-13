#pragma once
#include <Engine.h>

class PowerUp : public PhysicsObject {
    public:
        PowerUp();

    void update() override;
    virtual void onHitEffect();
    protected:
        bool effectTriggered = false; // Flag to ensure the effect is triggered only once
};