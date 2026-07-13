#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <functional>

class Window {
public:
    Window(int width, int height, const char* title);
    ~Window();

    void OnUpdate();
    bool ShouldClose() const;
    bool isKeyPressed(int key) const;

    int getWidth()  const { return m_Width; }
    int getHeight() const { return m_Height; }

    std::function<void(int, int)> onResize;

private:
    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
    GLFWwindow* m_Window;
    int m_Width, m_Height;
};