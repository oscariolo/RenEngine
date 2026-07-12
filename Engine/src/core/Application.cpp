#include "Application.h"
#include <iostream>
#include <chrono>

Application::Application() {
    m_Window = std::make_unique<Window>(720, 720, "My Engine");
}

Application::~Application() {}

void Application::Run() {
    std::cout << "Engine starting...\n";
    
    //Inicializacion
    glEnable(GL_DEPTH_TEST);
    OnInit(); 

    auto lastTime = std::chrono::steady_clock::now();

    while (m_Running && !m_Window->ShouldClose()) {
        // Calculate Delta Time
        auto currentTime = std::chrono::steady_clock::now();
        double deltaTime = std::chrono::duration<double>(currentTime - lastTime).count(); // Convert to seconds
        lastTime = currentTime;

        if(deltaTime > 0.25) {
            deltaTime = 0.25; //Limita deltaTime (espiral de la muerte)
        }

        // 2. Fijo a FPS 
        accumulator += deltaTime;
        OnInput();
        if(!freezeUpdate){
            if (accumulator >= timePerFrame) {
                OnUpdate(timePerFrame); //
                accumulator -= timePerFrame;
            }
        }

        // Clear the screen
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 3. Call the Sandbox render hook
        OnRender();

        m_Window->OnUpdate();
    }
}