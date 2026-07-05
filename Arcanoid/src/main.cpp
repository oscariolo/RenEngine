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
    std::shared_ptr<GameObject> gameobject;
    Camera camera;

protected:
    void OnInit() override {
        std::cout << "Sandbox Initialized!\n";

        FPS = 60; // Set the desired FPS value here
        timePerFrame = 1.0f / FPS;
        glEnable(GL_DEPTH_TEST);
        Renderer::init();
        camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
        Renderer::setCamera(camera);
        shader = std::make_shared<Shader>();

        gameobject = std::make_shared<GameObject>(std::make_shared<Cube>());        





        


    }

    void OnUpdate(double deltaTime) override {
        gameobject->position.x += 1.0f * deltaTime; // Rotate 50 degrees per second around the Y-axis
    }

    void OnRender() override {
        Renderer::submit(shader, gameobject);
    }
};

// The entry point simply creates our Sandbox application and runs it.
int main() {
    SandboxApp app;
    app.Run();
    return 0;
}