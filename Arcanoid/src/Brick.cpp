#include "Brick.h"
#include "physics/PhysicsEngine.h"

Brick::Brick(float brickWidth, float brickHeight, float brickDepth) : PhysicsObject(std::make_shared<Cube>(), rp3d::BodyType::STATIC) {
    setTexture(&getSharedTexture());
    addCollider(PhysicsEngine::physicsCommon.createBoxShape(
        rp3d::Vector3(brickWidth * 0.5f, brickHeight * 0.5f, brickDepth * 0.5f)));
    setScale(glm::vec3(brickWidth, brickHeight, brickDepth));
    setMaterialProperties(0.0f, 0.5f, 1.0f);
    uniformColor = glm::vec3(0.5f, 0.5f, 0.5f); // Red color for the brick
}

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
        scale = glm::vec3(currentScale.x * (1.0f - destructionTimer * 2.0f), currentScale.y * (1.0f - destructionTimer * 2.0f), currentScale.z * (1.0f - destructionTimer * 2.0f)); // Shrink vertically
        if(destructionTimer >= 0.5f){ // Destroy after 0.5 seconds
            visible = false;
            //queue_free();
        }
    }
}