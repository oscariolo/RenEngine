#include <Engine.h>
#include "physics/PhysicsEngine.h"
#include <reactphysics3d/reactphysics3d.h>
#include <iostream>

// Include your ported engine classes here
// #include <Renderer/Sphere.h>
// #include <Renderer/Lighting.h>
// #include <Renderer/Shader.h> // previously shader_utils.h

class SandboxApp : public Application {
private:

    double m_Accumulator = 0.0;
    std::shared_ptr<Shader> shader;
    std::shared_ptr<PhysicsObject> ball;
    std::shared_ptr<PhysicsObject> block;
    std::shared_ptr<PointLight> pointLight;
    Camera camera;

protected:
    void OnInit() override {
        Renderer::init();
        PhysicsEngine::init();

        std::cout << "Sandbox Initialized!\n";
        FPS = 60; // Set the desired FPS 
        timePerFrame = 1.0f / FPS;
        glEnable(GL_DEPTH_TEST);


        camera = Camera(glm::vec3(0.0f, 0.0f, 4.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
        Renderer::setCamera(camera);
        shader = std::make_shared<Shader>();

        pointLight = std::make_shared<PointLight>();

        pointLight->setPosition(-1.0f, 3.0f, 0.0f);
        Renderer::addPointLight(pointLight);


        ball = std::make_shared<PhysicsObject>(std::make_shared<Sphere>(),rp3d::BodyType::DYNAMIC);
        block = std::make_shared<PhysicsObject>(std::make_shared<Cube>(),rp3d::BodyType::STATIC);
        
        ball->addCollider(PhysicsEngine::physicsCommon.createSphereShape(0.5));
        block->addCollider(PhysicsEngine::physicsCommon.createBoxShape(rp3d::Vector3(0.5,0.5,0.5)));

        
        ball->setPosition(0,5,0);

        block->setPosition(0,-1,0);


    }

    void OnUpdate(double deltaTime) override {
        //La pelota es fuente de luz, mover la pelota de arriba a abajo y posicion de luz como la mimsa pelota
        //pointLight->setPosition(ball->position.x, ball->position.y, ball->position.z);
        
        PhysicsEngine::update(static_cast<float>(deltaTime));
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