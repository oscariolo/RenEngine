#include "Ball.h"
#include "physics/PhysicsEngine.h"


Ball::Ball(float radius) : PhysicsObject(std::make_shared<Sphere>(), rp3d::BodyType::DYNAMIC) {
    scale = glm::vec3(radius);
    addCollider(PhysicsEngine::physicsCommon.createSphereShape(radius));
    setMaterialProperties(1.0f, 0.0f, 1.0f);
    getRigidBody()->enableGravity(false);
    getRigidBody()->setLinearLockAxisFactor(rp3d::Vector3(1, 1, 0));
    getRigidBody()->setAngularLockAxisFactor(rp3d::Vector3(0, 0, 1));
    getRigidBody()->setLinearVelocity(rp3d::Vector3(0, -2, 0));
    texture = &getSharedTexture();
    pointLight = std::make_shared<PointLight>();
    pointLight->setPosition(-1.0f, 3.0f, 0.0f);
    Renderer::addPointLight(pointLight); // Add the point light to the renderer

}

Ball::~Ball() {
    Renderer::removePointLight(pointLight); // Remove the point light from the renderer when the ball is destroyed
}