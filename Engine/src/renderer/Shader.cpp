#include "Shader.h"
#include <glad/glad.h>

#include <fstream>
#include <sstream>
#include <iostream>

//si no se pasa ningun path, se toma el shader por defecto
Shader::Shader() : Shader("assets/shaders/base/vertex.vert", "assets/shaders/base/fragment.frag") {};

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
    // 1. Retrieve the vertex/fragment source code from filePath
    std::string vertexCode = ReadFile(vertexPath);
    std::string fragmentCode = ReadFile(fragmentPath);

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // 2. Compile shaders
    unsigned int vertex, fragment;

    // Vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    CheckCompileErrors(vertex, "VERTEX");

    // Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    CheckCompileErrors(fragment, "FRAGMENT");

    // 3. Shader Program
    m_RendererID = glCreateProgram();
    glAttachShader(m_RendererID, vertex);
    glAttachShader(m_RendererID, fragment);
    glLinkProgram(m_RendererID);
    CheckCompileErrors(m_RendererID, "PROGRAM");

    // 4. Delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader() {
    glDeleteProgram(m_RendererID);
}

void Shader::Bind() const {
    glUseProgram(m_RendererID);
}

void Shader::Unbind() const {
    glUseProgram(0);
}

std::string Shader::ReadFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << filepath << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void Shader::CheckCompileErrors(unsigned int shader, const std::string& type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}

// --- Uniform Setters ---
void Shader::SetInt(const std::string& name, int value) {
    glUniform1i(glGetUniformLocation(m_RendererID, name.c_str()), value);
}
void Shader::SetFloat(const std::string& name, float value) {
    glUniform1f(glGetUniformLocation(m_RendererID, name.c_str()), value);
}
void Shader::SetVec3(const std::string& name, const glm::vec3& value) {
    glUniform3fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, &value[0]);
}
void Shader::SetMat4(const std::string& name, const glm::mat4& value) {
    glUniformMatrix4fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}