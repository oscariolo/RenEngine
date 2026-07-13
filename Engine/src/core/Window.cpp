#include "Window.h"
#include "renderer/Renderer.h"
#include <iostream>

Window::Window(int width, int height, const char* title) 
    : m_Width(width), m_Height(height) {
    
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW\n";
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_Window = glfwCreateWindow(m_Width, m_Height, title, nullptr, nullptr);
    if (!m_Window) {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(m_Window);

    // GLAD must be initialized AFTER the context is made current
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD\n";
        return;
    }

    // Store a pointer to this Window so the static callback can reach m_Width/m_Height.
    glfwSetWindowUserPointer(m_Window, this);

    // This fires whenever the framebuffer is resized — including fullscreen
    // transitions — and updates both the stored dimensions and glViewport.
    glfwSetFramebufferSizeCallback(m_Window, framebufferResizeCallback);

    glViewport(0, 0, m_Width, m_Height);
}

void Window::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
    self->m_Width  = width;
    self->m_Height = height;

    glViewport(0, 0, width, height);
    Renderer::updateProjection(width, height);
}

Window::~Window() {
    glfwDestroyWindow(m_Window);
    glfwTerminate();
}

void Window::OnUpdate() {
    glfwPollEvents();
    glfwSwapBuffers(m_Window);
}

bool Window::ShouldClose() const {
    return glfwWindowShouldClose(m_Window);
}

bool Window::isKeyPressed(int key) const {
    return glfwGetKey(m_Window, key) == GLFW_PRESS;
}