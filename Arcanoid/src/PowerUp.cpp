#include "PowerUp.h"
#include <physics/PhysicsEngine.h>

PowerUp::PowerUp() : PhysicsObject() {
    setMesh(std::make_shared<Sphere>()); // Assuming you want to use a sphere mesh for the PowerUp
    getRigidBody()->setType(rp3d::BodyType::DYNAMIC); // Set the rigid body to dynamic
    scale = glm::vec3(0.3f); // Set a default scale for the PowerUp
    addCollider(PhysicsEngine::physicsCommon.createSphereShape(0.1f)); // Add a sphere collider
    getCollider()->setIsTrigger(true); // Set the collider to be a trigger

}

void PowerUp::update() {
    PhysicsObject::update(); // Call the base class update to sync position with physics
    // Additional logic for PowerUp can be added here, e.g., movement, lifetime, etc.
    getRigidBody()->setLinearVelocity(rp3d::Vector3(0, -1.0f, 0)); // Move downwards at a constant speed
}

void PowerUp::onHitEffect(){
    // Implement the effect that occurs when the PowerUp is hit
    // For example, you could spawn another ball or trigger a special effect
    std::cout << "PowerUp hit! Triggering effect..." << std::endl;
    // Additional logic for the effect can be added here
}
