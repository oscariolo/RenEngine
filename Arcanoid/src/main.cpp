#include <Engine.h>
#include "physics/PhysicsEngine.h"
#include <reactphysics3d/reactphysics3d.h>
#include <cmath>
#include <iostream>
#include <vector>

// Include your ported engine classes here
// #include <Renderer/Sphere.h>
// #include <Renderer/Lighting.h>
// #include <Renderer/Shader.h> // previously shader_utils.h

class SandboxApp : public Application {
private:

    double m_Accumulator = 0.0;
    std::shared_ptr<Shader> shader;
    std::shared_ptr<PhysicsObject> ball;
    std::shared_ptr<PhysicsObject> paddle;
    std::vector<std::shared_ptr<PhysicsObject>> bricks;
    std::vector<std::shared_ptr<PhysicsObject>> walls;
    std::shared_ptr<PointLight> pointLight;
    Camera camera;

    glm::vec3 TiltBoardPosition(const glm::vec3& point) const {
        const float tiltRadians = glm::radians(58.0f);
        float cosTilt = std::cos(tiltRadians);
        float sinTilt = std::sin(tiltRadians);
        return glm::vec3(
            point.x,
            point.y * cosTilt - point.z * sinTilt,
            point.y * sinTilt + point.z * cosTilt
        );
    }

protected:
    void OnInit() override {
        Renderer::init();
        PhysicsEngine::init();

        std::cout << "Sandbox Initialized!\n";
        FPS = 60; // Set the desired FPS 
        timePerFrame = 1.0f / FPS;
        glEnable(GL_DEPTH_TEST);
        camera = Camera(glm::vec3(0.0f, 0.0f, 5.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        Renderer::setCamera(camera);
        shader = std::make_shared<Shader>();

        pointLight = std::make_shared<PointLight>();

        pointLight->setPosition(-1.0f, 3.0f, 0.0f);
        Renderer::addPointLight(pointLight);

        constexpr float ballRadius = 0.18f;
        constexpr glm::vec3 paddleScale(1.10f, 0.18f, 0.28f);
        constexpr float playfieldHalfWidth = 2.6f;
        constexpr float playfieldHalfHeight = 3.0f;
        constexpr float wallThickness = 0.12f;
        constexpr float wallDepth = 0.35f;
        constexpr float brickWidth = 0.32f;
        constexpr float brickHeight = 0.18f;
        constexpr float brickDepth = 0.30f;

        ball = std::make_shared<PhysicsObject>(std::make_shared<Sphere>(), rp3d::BodyType::DYNAMIC);
        paddle = std::make_shared<PhysicsObject>(std::make_shared<Cube>(), rp3d::BodyType::STATIC);
        paddle->scale = paddleScale;
        paddle->addCollider(PhysicsEngine::physicsCommon.createBoxShape(
            rp3d::Vector3(paddleScale.x * 0.5f, paddleScale.y * 0.5f, paddleScale.z * 0.5f)));

        ball->scale = glm::vec3(ballRadius * 2.0f, ballRadius * 2.0f, ballRadius * 2.0f);
        ball->addCollider(PhysicsEngine::physicsCommon.createSphereShape(ballRadius));

        auto createWall = [&](const glm::vec3& pos, const glm::vec3& scale) {
            auto wall = std::make_shared<PhysicsObject>(std::make_shared<Cube>(), rp3d::BodyType::STATIC);

            wall->setPosition(pos.x, pos.y, pos.z);
            wall->setRotation(0.0f, 0.0f, 0.0f);
            wall->scale = scale;
            wall->addCollider(PhysicsEngine::physicsCommon.createBoxShape(
                rp3d::Vector3(scale.x * 0.5f, scale.y * 0.5f, scale.z * 0.5f)));

            walls.push_back(wall);
        };

        auto createBrick = [&](const glm::vec3& pos) {
            auto brick = std::make_shared<PhysicsObject>(std::make_shared<Cube>(), rp3d::BodyType::STATIC);

            brick->setPosition(pos.x, pos.y, pos.z);
            brick->setRotation(0.0f, 0.0f, 0.0f);
            brick->scale = glm::vec3(brickWidth, brickHeight, brickDepth);
            brick->addCollider(PhysicsEngine::physicsCommon.createBoxShape(
                rp3d::Vector3(brickWidth * 0.5f, brickHeight * 0.5f, brickDepth * 0.5f)));

            bricks.push_back(brick);
        };

        createWall(glm::vec3(0.0f,  playfieldHalfHeight, 0.0f), glm::vec3(playfieldHalfWidth * 2.0f, wallThickness, wallDepth));
        // Crear pared de abajo
        createWall(glm::vec3(0.0f, -playfieldHalfHeight, 0.0f), glm::vec3(playfieldHalfWidth * 2.0f, wallThickness, wallDepth));
        createWall(glm::vec3(-playfieldHalfWidth, 0.0f, 0.0f), glm::vec3(wallThickness, playfieldHalfHeight * 2.0f, wallDepth));
        createWall(glm::vec3( playfieldHalfWidth, 0.0f, 0.0f), glm::vec3(wallThickness, playfieldHalfHeight * 2.0f, wallDepth));

        for (int row = 0; row < 4; ++row) {
            for (int column = 0; column < 8; ++column) {
                glm::vec3 position(-1.55f + column * 0.37f, 1.25f - row * 0.24f, 0.0f);
                createBrick(position);
            }
        }

        
        // Flat positions directly from your original math!
        paddle->setPosition(0.0f, -1.35f, 0.0f);
        paddle->setRotation(0.0f, 0.0f, 0.0f);

        ball->setPosition(0.0f, -0.25f, 0.0f);
        ball->setRotation(0.0f, 0.0f, 0.0f);

            // LINEAR: 1 = Free, 0 = Locked. Allow X and Y (gravity), lock Z.
        ball->getRigidBody()->setLinearLockAxisFactor(rp3d::Vector3(1.0f, 1.0f, 0.0f));

        // ANGULAR: 0 = Locked, 1 = Free. Prevent spinning on X and Y, allow spinning on Z.
        ball->getRigidBody()->setAngularLockAxisFactor(rp3d::Vector3(0.0f, 0.0f, 1.0f));

    }

    void OnUpdate(double deltaTime) override {
        (void)deltaTime;
        pointLight->setPosition(ball->getPosition().x, ball->getPosition().y, ball->getPosition().z);
        PhysicsEngine::update(static_cast<float>(timePerFrame));
    }

    void OnRender() override {
        for (const auto& wall : walls) {
            Renderer::submit(shader, wall);
        }
        for (const auto& brick : bricks) {
            Renderer::submit(shader, brick);
        }
        Renderer::submit(shader, ball);
        Renderer::submit(shader, paddle);
    }
};

// The entry point simply creates our Sandbox application and runs it.
int main() {
    SandboxApp app;
    app.Run();
    return 0;
}