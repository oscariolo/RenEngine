#include <Engine.h>
#include <iostream>

// Include your ported engine classes here
// #include <Renderer/Sphere.h>
// #include <Renderer/Lighting.h>
// #include <Renderer/Shader.h> // previously shader_utils.h

class SandboxApp : public Application {
private:

    double m_Accumulator = 0.0;
    std::shared_ptr<Shader> shader;
    std::shared_ptr<GameObject> ball;
    std::shared_ptr<GameObject> block;
    std::shared_ptr<PointLight> pointLight;
    Camera camera;

protected:
    void OnInit() override {
        std::cout << "Sandbox Initialized!\n";

        FPS = 60; // Set the desired FPS value here
        timePerFrame = 1.0f / FPS;
        glEnable(GL_DEPTH_TEST);
        Renderer::init();
        camera = Camera(glm::vec3(0.0f, 0.0f, 4.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
        Renderer::setCamera(camera);
        shader = std::make_shared<Shader>();

        pointLight = std::make_shared<PointLight>();

        pointLight->setPosition(-1.0f, 3.0f, 0.0f);
        Renderer::addPointLight(pointLight);


        ball = std::make_shared<GameObject>(std::make_shared<Sphere>());
        block = std::make_shared<GameObject>(std::make_shared<Cube>());

        block->position = glm::vec3(0.0f, -1.5f, 0.0f);




        


    }

    void OnUpdate(double deltaTime) override {
        //La pelota es fuente de luz, mover la pelota de arriba a abajo y posicion de luz como la mimsa pelota
        float speed = 2.0f; // Adjust the speed as needed
        ball->position.y += speed * static_cast<float>(deltaTime);
        if (ball->position.y > 1.5f) {
            ball->position.y = -1.5f; // Reset to the bottom
        }
        pointLight->setPosition(ball->position.x, ball->position.y, ball->position.z);
        


    }

    void OnRender() override {
        Renderer::submit(shader, ball);
        Renderer::submit(shader, block);
    }
};

// The entry point simply creates our Sandbox application and runs it.
int main() {
    SandboxApp app;
    app.Run();
    return 0;
}