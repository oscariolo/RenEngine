#include <Engine.h>
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
    std::shared_ptr<GameObject> ball;
    std::shared_ptr<GameObject> paddle;
    std::vector<std::shared_ptr<GameObject>> bricks;
    std::vector<std::shared_ptr<GameObject>> walls;
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
        std::cout << "Sandbox Initialized!\n";

        FPS = 60; // Set the desired FPS value here
        timePerFrame = 1.0f / FPS;
        glEnable(GL_DEPTH_TEST);
        Renderer::init();
        camera = Camera(glm::vec3(0.0f, 3.2f, -6.5f), glm::vec3(0.0f, -0.2f, 1.6f), glm::vec3(0.0f, 1.0f, 0.0f));
        Renderer::setCamera(camera);
        shader = std::make_shared<Shader>();

        pointLight = std::make_shared<PointLight>();

        pointLight->setPosition(-1.0f, 3.0f, 0.0f);
        Renderer::addPointLight(pointLight);


        ball = std::make_shared<GameObject>(std::make_shared<Sphere>());
        paddle = std::make_shared<GameObject>(std::make_shared<Cube>());

        const float boardTiltDegrees = 58.0f;

        const float playfieldHalfWidth = 2.6f;
        const float playfieldHalfHeight = 3.0f;
        const float wallThickness = 0.12f;
        const float wallDepth = 0.35f;
        const float brickWidth = 0.32f;
        const float brickHeight = 0.18f;
        const float brickDepth = 0.30f;

        auto createWall = [&](const glm::vec3& position, const glm::vec3& scale) {
            auto wall = std::make_shared<GameObject>(std::make_shared<Cube>());
            wall->position = TiltBoardPosition(position);
            wall->rotation.x = boardTiltDegrees;
            wall->scale = scale;
            walls.push_back(wall);
        };

        auto createBrick = [&](const glm::vec3& position) {
            auto brick = std::make_shared<GameObject>(std::make_shared<Cube>());
            brick->position = TiltBoardPosition(position);
            brick->rotation.x = boardTiltDegrees;
            brick->scale = glm::vec3(brickWidth, brickHeight, brickDepth);
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

        paddle->position = TiltBoardPosition(glm::vec3(0.0f, -1.35f, 0.0f));
        paddle->rotation.x = boardTiltDegrees;
        paddle->scale = glm::vec3(0.95f, 0.16f, 0.30f);

        ball->position = TiltBoardPosition(glm::vec3(0.0f, -0.25f, 0.0f));
        ball->rotation.x = boardTiltDegrees;
        ball->scale = glm::vec3(0.14f, 0.14f, 0.14f);




        


    }

    void OnUpdate(double deltaTime) override {
        (void)deltaTime;
        pointLight->setPosition(ball->position.x, ball->position.y, ball->position.z);
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