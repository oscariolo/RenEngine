#include "DoubleBall.h"
#include <Engine.h>
#include "Ball.h"
#include "physics/PhysicsEngine.h"

void DoubleBall::onHitEffect() {
    if(!effectTriggered) {
        auto *newBall = PhysicsEngine::spawn<Ball>(0.25f); // Create a new ball with the same radius
        newBall->setPosition(getPosition()); // Set the new ball's position to the current position of the DoubleBall
        effectTriggered = true; // Ensure the effect is only triggered once
    }
}