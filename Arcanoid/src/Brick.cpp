#include "Brick.h"


void Brick::killBrick() {
    if (!isBeingDestroyed) {
        isBeingDestroyed = true;
        destructionTimer = 0.0f;
    }
}

void Brick::reset() {
    isBeingDestroyed = false;
    destructionTimer = 0.0f;
    scale = glm::vec3(1.0f, 1.0f, 1.0f); // Reset scale to original
    getRigidBody()->setIsActive(true);
    visible = true;
    setScale(originalScale); // Reset to original scale
}

void Brick::update(){
    PhysicsObject::update(); // Call the base class update to sync position with physics
    if(isBeingDestroyed){
        getRigidBody()->setIsActive(false);
        destructionTimer += 1.0f/60.0f; // Assuming update is called at 60 FPS
        glm::vec3 currentScale = scale;
        scale = glm::vec3(currentScale.x, currentScale.y * (1.0f - destructionTimer * 2.0f), currentScale.z); // Shrink vertically
        if(destructionTimer >= 0.5f){ // Destroy after 0.5 seconds
            visible = false;
            //queue_free();
        }
    }
}