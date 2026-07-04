#pragma once
#include "Window.h"
#include <memory>

class Application {
public:
    Application();
    virtual ~Application(); // Must be virtual now

    void Run();

protected:
    // The Sandbox will override these functions!
    virtual void OnInit() {}
    virtual void OnUpdate(double deltaTime) {}
    virtual void OnRender() {}

    std::unique_ptr<Window> m_Window;
    bool m_Running = true;
};