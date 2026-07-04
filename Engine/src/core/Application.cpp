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
    OnInit(); 

    auto lastTime = std::chrono::steady_clock::now();

    while (m_Running && !m_Window->ShouldClose()) {
        // Calculate Delta Time
        auto currentTime = std::chrono::steady_clock::now();
        double deltaTime = std::chrono::duration(currentTime - lastTime).count();
        lastTime = currentTime;

        // 2. Call the Sandbox update hook
        OnUpdate(deltaTime);

        // Clear the screen
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 3. Call the Sandbox render hook
        OnRender();

        m_Window->OnUpdate();
    }
}