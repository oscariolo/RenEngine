#include <Engine.h>
#include "physics/PhysicsEngine.h"
#include <reactphysics3d/reactphysics3d.h>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <random>
#include <vector>

// Include your ported engine classes here
// #include <Renderer/Sphere.h>
// #include <Renderer/Lighting.h>
// #include <Renderer/Shader.h> // previously shader_utils.h

class SandboxApp : public Application {
private:
    // Game constants
    const float m_playfieldHalfWidth = 2.6f;
    const float m_playfieldHalfHeight = 3.0f;
    const float m_wallThickness = 0.12f;
    const glm::vec3 m_paddleScale = glm::vec3(1.5f, 0.18f, 0.2f);

    double m_Accumulator = 0.0;
    std::mt19937 m_RandomEngine{ std::random_device{}() };
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

        constexpr float ballRadius = 0.1f;
        constexpr float wallDepth = 0.35f;
        constexpr int brickColumns = 8;
        constexpr int brickRows = 5;
        constexpr float brickWidth = 0.3f;
        constexpr float brickHeight = 0.2f;
        constexpr float brickDepth = 0.3f;
        constexpr float brickGapX = 0.12f;
        constexpr float brickGapY = 0.05f;
        constexpr float brickTopMargin = 0.1f;
        constexpr float brickSideMargin = 0.18f;

        ball = std::make_shared<PhysicsObject>(std::make_shared<Sphere>(), rp3d::BodyType::DYNAMIC);
        paddle = std::make_shared<PhysicsObject>(std::make_shared<Cube>(), rp3d::BodyType::STATIC);
        paddle->scale = m_paddleScale;
        paddle->addCollider(PhysicsEngine::physicsCommon.createBoxShape(
            rp3d::Vector3(m_paddleScale.x * 0.5f, m_paddleScale.y * 0.5f, m_paddleScale.z * 0.5f)));

        ball->scale = glm::vec3(ballRadius * 2.0f, ballRadius * 2.0f, ballRadius * 2.0f);
        ball->addCollider(PhysicsEngine::physicsCommon.createSphereShape(ballRadius));
        
        paddle->setMaterialProperties(0.0f, 0.0f, 1.0f); // Set paddle material properties
        ball-> setMaterialProperties(1.0f, 0.0f, 1.0f); // Set ball material properties
        ball->getRigidBody()->enableGravity(false); // Disable gravity for the ball
        auto createWall = [&](const glm::vec3& pos, const glm::vec3& scale) {
            auto wall = std::make_shared<PhysicsObject>(std::make_shared<Cube>(), rp3d::BodyType::STATIC);
            wall->addCollider(PhysicsEngine::physicsCommon.createBoxShape(
                rp3d::Vector3(scale.x * 0.5f, scale.y * 0.5f, scale.z * 0.5f)));
            wall->setMaterialProperties(0.0f, 0.5f, 1.0f); // Set wall material properties
            wall->setPosition(pos.x, pos.y, pos.z);
            wall->setRotation(0.0f, 0.0f, 0.0f);
            wall->scale = scale;


            walls.push_back(wall);
        };

        auto createBrick = [&](const glm::vec3& pos) {
            auto brick = std::make_shared<PhysicsObject>(std::make_shared<Cube>(), rp3d::BodyType::STATIC);
            brick->addCollider(PhysicsEngine::physicsCommon.createBoxShape(
            rp3d::Vector3(brickWidth * 0.5f, brickHeight * 0.5f, brickDepth * 0.5f)));
            brick->setMaterialProperties(0.0f, 0.5f, 1.0f); // Set brick material properties
            brick->setPosition(pos.x, pos.y, pos.z);
            brick->setRotation(0.0f, 0.0f, 0.0f);
            brick->scale = glm::vec3(brickWidth, brickHeight, brickDepth);
            bricks.push_back(brick);
        };

        createWall(glm::vec3(0.0f,  m_playfieldHalfHeight, 0.0f), glm::vec3(m_playfieldHalfWidth * 2.0f, m_wallThickness, wallDepth));
        // Crear pared de abajo
        createWall(glm::vec3(0.0f, -m_playfieldHalfHeight, 0.0f), glm::vec3(m_playfieldHalfWidth * 2.0f, m_wallThickness, wallDepth));
        createWall(glm::vec3(-m_playfieldHalfWidth, 0.0f, 0.0f), glm::vec3(m_wallThickness, m_playfieldHalfHeight * 2.0f, wallDepth));
        createWall(glm::vec3( m_playfieldHalfWidth, 0.0f, 0.0f), glm::vec3(m_wallThickness, m_playfieldHalfHeight * 2.0f, wallDepth));

        const float usableWidth = (m_playfieldHalfWidth - m_wallThickness * 0.5f - brickSideMargin) * 2.0f;
        const float usableHeight = (m_playfieldHalfHeight - m_wallThickness * 0.5f - brickTopMargin) - (-0.55f);
        const float horizontalSpacing = std::max(brickGapX, (usableWidth - brickColumns * brickWidth) / std::max(1, brickColumns - 1));
        const float verticalSpacing = std::max(brickGapY, (usableHeight - brickRows * brickHeight) / std::max(1, brickRows - 1));
        const float totalBrickWidth = brickColumns * brickWidth + (brickColumns - 1) * horizontalSpacing;
        const float startX = -totalBrickWidth * 0.5f + brickWidth * 0.5f;
        const float startY = m_playfieldHalfHeight - m_wallThickness - brickTopMargin - brickHeight * 0.5f;

        for (int row = 0; row < brickRows; ++row) {
            for (int column = 0; column < brickColumns; ++column) {
                glm::vec3 position(
                    startX + column * (brickWidth + horizontalSpacing),
                    startY - row * (brickHeight + verticalSpacing),
                    0.0f
                );
                createBrick(position);
            }
        }

        
        // Flat positions directly from your original math!
        paddle->setPosition(0.0f, -2.00f, 0.0f);
        paddle->setRotation(0.0f, 0.0f, 0.0f);

        ball->setPosition(0.0f, -1.0f, 0.0f);
        ball->setRotation(0.0f, 0.0f, 0.0f);

            // LINEAR: 1 = Free, 0 = Locked. Allow X and Y (gravity), lock Z.
        ball->getRigidBody()->setLinearLockAxisFactor(rp3d::Vector3(1.0f, 1.0f, 0.0f));

        const float ballSpeed = 4.0f; // Adjust this value to control the speed of the ball
        // ANGULAR: 0 = Locked, 1 = Free. Prevent spinning on X and Y, allow spinning on Z.
        ball->getRigidBody()->setAngularLockAxisFactor(rp3d::Vector3(0.0f, 0.0f, 1.0f));
        ball->getRigidBody()->setLinearVelocity(rp3d::Vector3(0.0f, ballSpeed, 0.0f)); // Set initial velocity for the ball
    
        //logica de desaparicion de ladrillos

        auto onBrickHit = [&](PhysicsObject* brick, PhysicsObject* ball) {
            brick->queue_free(); // Mark the brick for deletion
        };

        for (const auto& brick : bricks) {
            brick->onCollisionCallback(onBrickHit);
        }

    
    }

    void OnInput() override {
        const float paddleSpeed = 5.0f;
        glm::vec3 currentPosition = paddle->getPosition(); //obtengo la posicion actual del paddle

        if (m_Window->isKeyPressed(GLFW_KEY_A)) {
            currentPosition.x -= paddleSpeed * timePerFrame;
        }
        if (m_Window->isKeyPressed(GLFW_KEY_D)) {
            currentPosition.x += paddleSpeed * timePerFrame;
        }

        const float paddleHalfWidth = paddle->scale.x * 0.5f;
        const float maxPaddleX = m_playfieldHalfWidth - m_wallThickness - paddleHalfWidth;
        
        currentPosition.x = std::max(-maxPaddleX, std::min(maxPaddleX, currentPosition.x));
        
        paddle->setPosition(currentPosition);
    }

    void OnUpdate(double deltaTime) override {
        m_Accumulator += deltaTime;

        if (m_Accumulator >= 1.25) {
            m_Accumulator = 0.0;

            const rp3d::Vector3 linearVelocity = ball->getRigidBody()->getLinearVelocity();
            const float horizontalSpeed = std::abs(static_cast<float>(linearVelocity.x));
            const float randomValue = std::uniform_real_distribution<float>(0.0f, 1.0f)(m_RandomEngine);

            if (horizontalSpeed < 0.75f || randomValue < 0.35f) {
                const float pushDirection = (std::uniform_real_distribution<float>(0.0f, 1.0f)(m_RandomEngine) < 0.5f) ? -1.0f : 1.0f;
                const float pushAmount = 1.75f;
                const float newXVelocity = linearVelocity.x + pushDirection * pushAmount;
                ball->getRigidBody()->setLinearVelocity(rp3d::Vector3(newXVelocity, linearVelocity.y, 0.0f));
            }
        }

        pointLight->setPosition(ball->getPosition().x, ball->getPosition().y, ball->getPosition().z);

        PhysicsEngine::update(static_cast<float>(timePerFrame));

        bricks.erase(
            std::remove_if(bricks.begin(), bricks.end(),
                [](const std::shared_ptr<PhysicsObject>& brick){
                    return brick->m_isQueuedForDeletion;
                }),
            bricks.end());
    }

    void OnRender() override {
        for (const auto& wall : walls) {
            Renderer::submit(shader.get(), wall.get());
        }
        for (const auto& brick : bricks) {
            Renderer::submit(shader.get(), brick.get());
        }
        Renderer::submit(shader.get(), ball.get());
        Renderer::submit(shader.get(), paddle.get());
    }
};

// The entry point simply creates our Sandbox application and runs it.
int main() {
    SandboxApp app;
    app.Run();
    return 0;
}