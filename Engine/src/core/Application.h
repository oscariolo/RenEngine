#pragma once
#include "Window.h"
#include <memory>

class Application {
public:
    Application();
    virtual ~Application(); 

    void Run();

protected:
    //Bucle para el juego y la logica del juego
    virtual void OnInit() {}
    virtual void OnInput() {}
    virtual void OnUpdate(double deltaTime) {}
    virtual void OnRender() {}

    std::unique_ptr<Window> m_Window;
    bool m_Running = true;
    int FPS = 60;
    float timePerFrame = 1.0f / FPS;
    double accumulator = 0.0f;
    
};