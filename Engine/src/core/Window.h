#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window {
public:
    Window(int width, int height, const char* title);
    ~Window();

    void OnUpdate();
    bool ShouldClose() const;

private:
    GLFWwindow* m_Window;
    int m_Width, m_Height;
};