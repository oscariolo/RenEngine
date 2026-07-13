#include "renderer/TextRenderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <vector>

// Define STB_TRUETYPE_IMPLEMENTATION in one C++ file
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

TextRenderer::TextRenderer(unsigned int width, unsigned int height)
    : TextShader("assets/shaders/text/vertex.vert", "assets/shaders/text/fragment.frag")
{
    // Configure shader uniforms for a bottom-left origin projection
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height));
    this->TextShader.Bind();
    this->TextShader.SetMat4("projection", projection);
    this->TextShader.SetInt("text", 0);
    this->TextShader.Unbind();

    // Configure VAO/VBO for texture quads
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

TextRenderer::~TextRenderer()
{
    glDeleteVertexArrays(1, &this->VAO);
    glDeleteBuffers(1, &this->VBO);
    glDeleteTextures(1, &this->fontTexture);
}

void TextRenderer::updateProjection(unsigned int width, unsigned int height)
{
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height));
    this->TextShader.Bind();
    this->TextShader.SetMat4("projection", projection);
    this->TextShader.Unbind();
}

void TextRenderer::Init(const char* fontPath, unsigned int fontSize)
{
    // Read font file
    std::ifstream fontFile(fontPath, std::ios::binary);
    if (!fontFile.is_open()) {
        std::cout << "ERROR::TEXTRENDERER: Failed to open font file: " << fontPath << std::endl;
        return;
    }
    std::vector<unsigned char> fontBuffer((std::istreambuf_iterator<char>(fontFile)), std::istreambuf_iterator<char>());
    fontFile.close();

    // Prepare a bitmap buffer for the ASCII character set
    const int bitmapWidth = 512;
    const int bitmapHeight = 512;
    static unsigned char fontBitmap[bitmapWidth * bitmapHeight];
    
    // Bake the font characters into the bitmap and store character data in this->cdata
    stbtt_BakeFontBitmap(fontBuffer.data(), 0, (float)fontSize, fontBitmap, bitmapWidth, bitmapHeight, 32, 96, this->cdata);

    // Generate a texture for the font atlas
    glGenTextures(1, &this->fontTexture);
    glBindTexture(GL_TEXTURE_2D, this->fontTexture);
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RED, bitmapWidth, bitmapHeight, 0, GL_RED, GL_UNSIGNED_BYTE, fontBitmap
    );
    
    // Set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void TextRenderer::RenderText(std::string text, float x, float y, float scale, glm::vec3 color)
{
    // Enable blending for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // glDisable(GL_DEPTH_TEST); // Disable depth testing for text rendering

    // Activate corresponding render state	
    this->TextShader.Bind();
    this->TextShader.SetVec3("textColor", color);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(this->VAO);
    glBindTexture(GL_TEXTURE_2D, this->fontTexture);

    const float atlasWidth = 512.0f;
    const float atlasHeight = 512.0f;

    float current_x = x;

    for (char c : text)
    {
        if (c >= 32 && c < 128)
        {
            stbtt_bakedchar* b = &cdata[c - 32];

            // Calculate character quad dimensions and position
            float x0 = current_x + b->xoff * scale;
            float y0 = y - b->yoff * scale;
            float x1 = x0 + (b->x1 - b->x0) * scale;
            float y1 = y0 - (b->y1 - b->y0) * scale;
            
            // Normalize texture coordinates
            float s0 = b->x0 / atlasWidth;
            float t0 = b->y0 / atlasHeight;
            float s1 = b->x1 / atlasWidth;
            float t1 = b->y1 / atlasHeight;

            // Build vertex data for the quad
            float vertices[6][4] = {
                { x0, y0, s0, t0 },
                { x1, y1, s1, t1 },
                { x0, y1, s0, t1 },

                { x0, y0, s0, t0 },
                { x1, y0, s1, t0 },
                { x1, y1, s1, t1 }
            };
            
            // Update VBO memory
            glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            // Render the quad
            glDrawArrays(GL_TRIANGLES, 0, 6);

            // Advance cursor for the next character
            current_x += b->xadvance * scale;
        }
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    this->TextShader.Unbind();
    
    // glEnable(GL_DEPTH_TEST); // Re-enable depth testing after text rendering
    glDisable(GL_BLEND);
}

float TextRenderer::GetTextWidth(const std::string& text, float scale) const
{
    float width = 0.0f;
    for (char c : text)
    {
        if (c >= 32 && c < 128)
        {
            const stbtt_bakedchar* b = &cdata[c - 32];
            width += b->xadvance * scale;
        }
    }
    return width;
}