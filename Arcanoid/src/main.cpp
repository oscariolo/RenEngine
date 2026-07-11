#include <Engine.h>
#include "physics/PhysicsEngine.h"
#include <reactphysics3d/reactphysics3d.h>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <random>
#include <vector>

class SandboxApp : public Application {
private:
    double m_Accumulator = 0.0;
    std::mt19937 m_RandomEngine{ std::random_device{}() };
    std::shared_ptr<Shader> shader;
    std::shared_ptr<PointLight> pointLight;
    Camera camera;

    // Raw observer pointers — PhysicsEngine owns the memory.
    PhysicsObject* ball   = nullptr;
    PhysicsObject* paddle = nullptr;
    std::vector<PhysicsObject*> bricks;
    std::vector<PhysicsObject*> walls;

protected:
    void OnInit() override {
        Renderer::init();
        PhysicsEngine::init();

        FPS = 60;
        timePerFrame = 1.0f / FPS;
        glEnable(GL_DEPTH_TEST);

        camera = Camera(glm::vec3(0.0f, 0.0f, 5.5f),
                        glm::vec3(0.0f, 0.0f, 0.0f),
                        glm::vec3(0.0f, 1.0f, 0.0f));
        Renderer::setCamera(camera);
        shader = std::make_shared<Shader>();

        pointLight = std::make_shared<PointLight>();
        pointLight->setPosition(-1.0f, 3.0f, 0.0f);
        Renderer::addPointLight(pointLight);

        constexpr float     ballRadius          = 0.1f;
        constexpr glm::vec3 paddleScale         = glm::vec3(1.5f, 0.18f, 0.2f);
        constexpr float     playfieldHalfWidth  = 2.6f;
        constexpr float     playfieldHalfHeight = 3.0f;
        constexpr float     wallThickness       = 0.12f;
        constexpr float     wallDepth           = 0.35f;
        constexpr int       brickColumns        = 8;
        constexpr int       brickRows           = 5;
        constexpr float     brickWidth          = 0.3f;
        constexpr float     brickHeight         = 0.2f;
        constexpr float     brickDepth          = 0.3f;
        constexpr float     brickGapX           = 0.12f;
        constexpr float     brickGapY           = 0.05f;
        constexpr float     brickTopMargin      = 0.1f;
        constexpr float     brickSideMargin     = 0.18f;
        constexpr float     ballSpeed           = 4.0f;

        // --- Ball ---
        ball = PhysicsEngine::spawn<PhysicsObject>(std::make_shared<Sphere>(), rp3d::BodyType::DYNAMIC);
        ball->scale = glm::vec3(ballRadius * 2.0f);
        ball->addCollider(PhysicsEngine::physicsCommon.createSphereShape(ballRadius));
        ball->setMaterialProperties(1.0f, 0.0f, 1.0f);
        ball->getRigidBody()->enableGravity(false);
        ball->getRigidBody()->setLinearLockAxisFactor(rp3d::Vector3(1, 1, 0));
        ball->getRigidBody()->setAngularLockAxisFactor(rp3d::Vector3(0, 0, 1));
        ball->setPosition(0.0f, -1.0f, 0.0f);
        ball->getRigidBody()->setLinearVelocity(rp3d::Vector3(0, ballSpeed, 0));

        // --- Paddle ---
        paddle = PhysicsEngine::spawn<PhysicsObject>(std::make_shared<Cube>(), rp3d::BodyType::STATIC);
        paddle->scale = paddleScale;
        paddle->addCollider(PhysicsEngine::physicsCommon.createBoxShape(
            rp3d::Vector3(paddleScale.x * 0.5f, paddleScale.y * 0.5f, paddleScale.z * 0.5f)));
        paddle->setMaterialProperties(0.0f, 0.0f, 1.0f);
        paddle->setPosition(0.0f, -2.0f, 0.0f);

        // --- Walls ---
        auto spawnWall = [&](glm::vec3 pos, glm::vec3 s) {
            auto* w = PhysicsEngine::spawn<PhysicsObject>(std::make_shared<Cube>(), rp3d::BodyType::STATIC);
            w->scale = s;
            w->addCollider(PhysicsEngine::physicsCommon.createBoxShape(
                rp3d::Vector3(s.x * 0.5f, s.y * 0.5f, s.z * 0.5f)));
            w->setMaterialProperties(0.0f, 0.5f, 1.0f);
            w->setPosition(pos.x, pos.y, pos.z);
            walls.push_back(w);
        };
        spawnWall({0,  playfieldHalfHeight, 0}, {playfieldHalfWidth*2, wallThickness, wallDepth});
        spawnWall({0, -playfieldHalfHeight, 0}, {playfieldHalfWidth*2, wallThickness, wallDepth});
        spawnWall({-playfieldHalfWidth, 0,  0}, {wallThickness, playfieldHalfHeight*2, wallDepth});
        spawnWall({ playfieldHalfWidth, 0,  0}, {wallThickness, playfieldHalfHeight*2, wallDepth});

        // --- Bricks ---
        const float usableWidth  = (playfieldHalfWidth - wallThickness*0.5f - brickSideMargin) * 2.0f;
        const float usableHeight = (playfieldHalfHeight - wallThickness*0.5f - brickTopMargin) - (-0.55f);
        const float hSpacing     = std::max(brickGapX, (usableWidth  - brickColumns*brickWidth)  / std::max(1, brickColumns-1));
        const float vSpacing     = std::max(brickGapY, (usableHeight - brickRows*brickHeight)    / std::max(1, brickRows-1));
        const float totalW       = brickColumns*brickWidth + (brickColumns-1)*hSpacing;
        const float startX       = -totalW*0.5f + brickWidth*0.5f;
        const float startY       = playfieldHalfHeight - wallThickness - brickTopMargin - brickHeight*0.5f;

        for(int row = 0; row < brickRows; ++row){
            for(int col = 0; col < brickColumns; ++col){
                auto* brick = PhysicsEngine::spawn<PhysicsObject>(std::make_shared<Cube>(), rp3d::BodyType::STATIC);
                brick->scale = glm::vec3(brickWidth, brickHeight, brickDepth);
                brick->addCollider(PhysicsEngine::physicsCommon.createBoxShape(
                    rp3d::Vector3(brickWidth*0.5f, brickHeight*0.5f, brickDepth*0.5f)));
                brick->setMaterialProperties(0.0f, 0.5f, 1.0f);
                brick->setPosition(
                    startX + col*(brickWidth + hSpacing),
                    startY - row*(brickHeight + vSpacing),
                    0.0f);
                brick->onCollisionCallback([](PhysicsObject* self, PhysicsObject*){
                    self->queue_free();
                });
                bricks.push_back(brick);
            }
        }

        PhysicsEngine::onBeforeDestroy = [this](PhysicsObject* obj){
            bricks.erase(std::remove(bricks.begin(), bricks.end(), obj), bricks.end());
        };
    }

    void OnUpdate(double deltaTime) override {
        m_Accumulator += deltaTime;
        if(m_Accumulator >= 1.25){
            m_Accumulator = 0.0;
            const rp3d::Vector3 vel    = ball->getRigidBody()->getLinearVelocity();
            const float         hSpeed = std::abs(static_cast<float>(vel.x));
            const float         rnd    = std::uniform_real_distribution<float>(0,1)(m_RandomEngine);
            if(hSpeed < 0.75f || rnd < 0.35f){
                const float dir = (std::uniform_real_distribution<float>(0,1)(m_RandomEngine) < 0.5f) ? -1.0f : 1.0f;
                ball->getRigidBody()->setLinearVelocity(rp3d::Vector3(vel.x + dir*1.75f, vel.y, 0));
            }
        }

        pointLight->setPosition(ball->getPosition().x, ball->getPosition().y, ball->getPosition().z);

        PhysicsEngine::update(static_cast<float>(timePerFrame));
    }

    void OnRender() override {
        for(auto* w : walls)  Renderer::submit(shader.get(), w);
        for(auto* b : bricks){
            if(!b->m_isQueuedForDeletion) Renderer::submit(shader.get(), b);
        }
        Renderer::submit(shader.get(), ball);
        Renderer::submit(shader.get(), paddle);
    }
};

int main() {
    SandboxApp app;
    app.Run();
    return 0;
}