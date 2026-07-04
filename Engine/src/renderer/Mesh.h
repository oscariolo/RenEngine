#pragma once
#include <vector>
#include "Vertex.h"

class Mesh{

    public:
        Mesh();
        ~Mesh();

        void generateVertices();

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        unsigned int ID;
        
        std::vector<Vertex> getVertices() const;
        std::vector<unsigned int> getIndices() const;

};