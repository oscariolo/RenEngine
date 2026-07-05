#pragma once

#include <string>
#include <glm/glm.hpp>

class Shader {
public:
    // Pass the full relative path, e.g., "assets/shaders/lighting.vert"
    Shader();
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    // Uniform utility functions
    void SetInt(const std::string& name, int value);
    void SetFloat(const std::string& name, float value);
    void SetVec3(const std::string& name, const glm::vec3& value);
    void SetMat4(const std::string& name, const glm::mat4& value);

private:
    unsigned int m_RendererID;

    // Helper functions
    std::string ReadFile(const std::string& filepath);
    void CheckCompileErrors(unsigned int shader, const std::string& type);
};