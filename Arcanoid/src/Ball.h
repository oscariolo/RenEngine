#pragma once
#include <Engine.h>
#include "physics/PhysicsEngine.h"

class Ball : public PhysicsObject {
    public:
        ~Ball() override;
        Ball(float radius = 0.5f);
        void update() override {
            if (pointLight) {
                pointLight->setPosition(getPosition().x, getPosition().y, getPosition().z); // Update the point light's position to match the ball's position
            }
            setRotation(getRigidBody()->getLinearVelocity().x * rotationSpeed * glfwGetTime(), getRigidBody()->getLinearVelocity().y * rotationSpeed * glfwGetTime(), getRigidBody()->getLinearVelocity().z * rotationSpeed * glfwGetTime());
            PhysicsObject::update(); // Call the base class update to sync position with physics
        }
    private:
        const float rotationSpeed = 2.0f; // Adjust this value to control the rotation speed
        static Texture& getSharedTexture() {
            static Texture texture = [](){
                Texture t;
                t.loadTexture("assets/textures/ball.jpg");
                return t;
            }();
            return texture;
        }
        std::shared_ptr<PointLight> pointLight; // Add a PointLight member to the Ball class
};