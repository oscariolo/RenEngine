#pragma once
#include "PowerUp.h"

class DoubleBall : public PowerUp {
public:
    using PowerUp::PowerUp; // Inherit constructors from PowerUp
    void update() override {
        PowerUp::update(); // Call the base class update to sync position with physics
        // Additional DoubleBall-specific update logic can go here
    }
    void onHitEffect() override;
};

