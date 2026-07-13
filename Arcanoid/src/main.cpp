#include <Engine.h>
#include "physics/PhysicsEngine.h"
#include <reactphysics3d/reactphysics3d.h>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <random>
#include <vector>
#include "Brick.h"
#include "renderer/TextRenderer.h"

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
    std::shared_ptr<PointLight> pointLight;
    Camera camera;
    Texture brickTexture;
    TextRenderer* textRenderer;

    // Raw observer pointers — PhysicsEngine owns the memory.
    PhysicsObject* ball   = nullptr;
    PhysicsObject* paddle = nullptr;
    std::vector<Brick*> bricks;
    std::vector<PhysicsObject*> walls;

public:
    ~SandboxApp()
    {
        delete textRenderer;
    }

protected:
    void OnInit() override {
        Renderer::init();
        PhysicsEngine::init();

        FPS = 60;
        timePerFrame = 1.0f / FPS;
        glEnable(GL_DEPTH_TEST);

        camera = Camera(glm::vec3(0.0f, -7.0f, 6.0f),
                        glm::vec3(0.0f, 1.0f, 1.0f),
                        glm::vec3(0.0f, 1.0f, 0.0f));
        
        brickTexture.loadTexture("assets/textures/rene.jpg");

        textRenderer = new TextRenderer(m_Window->getWidth(), m_Window->getHeight());
        textRenderer->Init("assets/fonts/VCR_OSD_MONO_1.001.ttf", 48);
        
        // In OnInit, after constructing textRenderer:
        m_Window->onResize = [this](int w, int h) {
            textRenderer->updateProjection(w, h);
        };
                

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
        ball->scale = glm::vec3(ballRadius * 2.5f);
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
        paddle->onCollisionCallback([](PhysicsObject* self, PhysicsObject* other){
            if(other == nullptr) return;
            if(other->getRigidBody() == nullptr) return;
            if(self == nullptr) return;

            // Calculate the offset based on the collision point
            rp3d::Vector3 collisionPoint = other->getRigidBody()->getTransform().getPosition();
            rp3d::Vector3 paddlePosition = self->getRigidBody()->getTransform().getPosition();
            float offset = collisionPoint.x - paddlePosition.x;

            // Adjust the ball's velocity based on the offset
            rp3d::Vector3 currentVelocity = other->getRigidBody()->getLinearVelocity();
            float speed = currentVelocity.length();
            float newDirectionX = offset * 2.0f; // Scale the offset for more pronounced effect
            rp3d::Vector3 newVelocity(newDirectionX, currentVelocity.y, 0);
            newVelocity.normalize();
            newVelocity *= speed; // Maintain the original speed

            other->getRigidBody()->setLinearVelocity(newVelocity);
        });

        // --- Walls ---
        auto spawnWall = [&](glm::vec3 pos, glm::vec3 s) {
            auto* w = PhysicsEngine::spawn<PhysicsObject>(std::make_shared<Cube>(), rp3d::BodyType::STATIC);
            w->scale = s;
            w->addCollider(PhysicsEngine::physicsCommon.createBoxShape(
                rp3d::Vector3(s.x * 0.5f, s.y * 0.5f, s.z * 0.5f)));
            w->setMaterialProperties(0.0f, 0.5f, 1.0f);
            w->setPosition(pos.x, pos.y, pos.z);
            walls.push_back(w);
            return w;
        };
        spawnWall({0,  playfieldHalfHeight, 0}, {playfieldHalfWidth*2, wallThickness, wallDepth});
        auto bottomWall = spawnWall({0, -playfieldHalfHeight, 0}, {playfieldHalfWidth*2, wallThickness, wallDepth});
        bottomWall->onCollisionCallback([this](PhysicsObject* self, PhysicsObject* other){
            if(other == ball){
                freezeUpdate = true;
            };
        });
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
                auto* brick = PhysicsEngine::spawn<Brick>(std::make_shared<Cube>(), rp3d::BodyType::STATIC);
                brick->setTexture(&brickTexture);
                brick->setScale(glm::vec3(brickWidth, brickHeight, brickDepth));
                brick->addCollider(PhysicsEngine::physicsCommon.createBoxShape(
                    rp3d::Vector3(brickWidth*0.5f, brickHeight*0.5f, brickDepth*0.5f)));
                brick->setMaterialProperties(0.0f, 0.5f, 1.0f);
                brick->setPosition(
                    startX + col*(brickWidth + hSpacing),
                    startY - row*(brickHeight + vSpacing),
                    0.0f);
                brick->onCollisionCallback([](PhysicsObject* self, PhysicsObject*){
                    static_cast<Brick*>(self)->killBrick();
                });
                bricks.push_back(brick);
            }
        }

    }

    void OnInput() override {
        const float paddleSpeed = 5.0f;
        glm::vec3 currentPosition = paddle->getPosition(); //obtengo la posicion actual del paddle

        if (m_Window->isKeyPressed(GLFW_KEY_A) && !freezeUpdate) {
            currentPosition.x -= paddleSpeed * timePerFrame;
        }
        if (m_Window->isKeyPressed(GLFW_KEY_D) && !freezeUpdate) {
            currentPosition.x += paddleSpeed * timePerFrame;
        }

        if(freezeUpdate && m_Window->isKeyPressed(GLFW_KEY_R)){
            freezeUpdate = false;
            ball->setPosition(0.0f, -1.0f, 0.0f);
            ball->getRigidBody()->setLinearVelocity(rp3d::Vector3(0, -4.0f, 0));
            paddle->setPosition(0.0f, -2.0f, 0.0f);
            for(auto* b : bricks){
                b->reset();
            }
        }

        const float paddleHalfWidth = paddle->scale.x * 0.5f;
        const float maxPaddleX = m_playfieldHalfWidth - m_wallThickness - paddleHalfWidth;
        
        currentPosition.x = std::max(-maxPaddleX, std::min(maxPaddleX, currentPosition.x));
        
        paddle->setPosition(currentPosition);
        ball->getRigidBody()->setAngularVelocity(rp3d::Vector3(0, 0, 3.0f * currentPosition.x));
    }

    void OnUpdate(double deltaTime) override {

        pointLight->setPosition(ball->getPosition().x, ball->getPosition().y, ball->getPosition().z);

        PhysicsEngine::update(static_cast<float>(timePerFrame));
    }

    void OnRender() override {
        for(auto* w : walls)  Renderer::submit(shader.get(), w);
        for(auto* b : bricks){
            if(b->visible) Renderer::submit(shader.get(), b);
        }
        Renderer::submit(shader.get(), ball);
        Renderer::submit(shader.get(), paddle);

        textRenderer->RenderText("SCORE: 0  LIVES: 3", 5.0f, m_Window->getHeight() - 20.0f, 0.4f, glm::vec3(1.0, 1.0, 1.0f));

        if (freezeUpdate) {
            const float windowWidth = static_cast<float>(m_Window->getWidth());
            const float windowHeight = static_cast<float>(m_Window->getHeight());

            const std::string gameOverText = "GAME OVER!";
            const float gameOverScale = 1.2f;
            const float gameOverWidth = textRenderer->GetTextWidth(gameOverText, gameOverScale);
            const float gameOverX = (windowWidth - gameOverWidth) * 0.5f;
            const float gameOverY = windowHeight * 0.5f + 20.0f; // Slightly above center

            textRenderer->RenderText(gameOverText, gameOverX, gameOverY, gameOverScale, glm::vec3(1.0f, 0.2f, 0.2f));

            const std::string restartText = "Press 'R' to Restart";
            const float restartScale = 0.5f;
            const float restartWidth = textRenderer->GetTextWidth(restartText, restartScale);
            const float restartX = (windowWidth - restartWidth) * 0.5f;
            const float restartY = gameOverY - 60.0f;

            textRenderer->RenderText(restartText, restartX, restartY, restartScale, glm::vec3(1.0f, 1.0f, 1.0f));
        }
    }
};

int main() {
    SandboxApp app;
    app.Run();
    return 0;
}