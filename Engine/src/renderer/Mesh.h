#pragma once
#include <vector>
#include "Vertex.h"
#include "utils/Texture.h"
class Mesh{

    public:
        Mesh();
        virtual ~Mesh();

        void generateVertices();
        std::vector<Vertex> getVertices() const;
        std::vector<unsigned int> getIndices() const;
        unsigned int VAO_ID, VBO_ID, EBO_ID;
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
    
    protected:

        void setupBuffers();
        
};