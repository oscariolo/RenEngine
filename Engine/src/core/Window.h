#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window {
public:
    Window(int width, int height, const char* title);
    ~Window();

    void OnUpdate();
    bool ShouldClose() const;
    bool isKeyPressed(int key) const;

    int getWidth() const { return m_Width; }
    int getHeight() const { return m_Height; }

private:
    GLFWwindow* m_Window;
    int m_Width, m_Height;
};