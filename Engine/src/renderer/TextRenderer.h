#pragma once

#include <string>
#include <glm/glm.hpp>
#include "renderer/Shader.h"
#include "stb_truetype.h"

class TextRenderer
{
public:
    // Constructor
    TextRenderer(unsigned int width, unsigned int height);
    // Destructor
    ~TextRenderer();

    // Loads a font from file
    void Init(const char* fontPath, unsigned int fontSize);
    
    // Renders a string of text using the pre-compiled list of characters
    void RenderText(std::string text, float x, float y, float scale, glm::vec3 color = glm::vec3(1.0f));

    float GetTextWidth(const std::string& text, float scale) const;
    
    void updateProjection(unsigned int width, unsigned int height);
    
private:
    // Render state
    unsigned int VAO, VBO;
    unsigned int fontTexture;
    stbtt_bakedchar cdata[96]; // ASCII 32..127 is 96 characters
    Shader TextShader;
};