#include <Engine.h>
#include <iostream>

// Include your ported engine classes here
// #include <Renderer/Sphere.h>
// #include <Renderer/Lighting.h>
// #include <Renderer/Shader.h> // previously shader_utils.h

class SandboxApp : public Application {
private:

    double m_Accumulator = 0.0;

protected:
    void OnInit() override {
        std::cout << "Sandbox Initialized!\n";
        glEnable(GL_DEPTH_TEST);

    }

    void OnUpdate(double deltaTime) override {

    }

    void OnRender() override {

    }
};

// The entry point simply creates our Sandbox application and runs it.
int main() {
    SandboxApp app;
    app.Run();
    return 0;
}