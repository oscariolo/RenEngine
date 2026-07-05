#pragma once
#include "Mesh.h"

class Sphere : public Mesh {
public:
    Sphere(unsigned int stacks = 24, unsigned int sectors = 36, float radius = 0.5f);
    ~Sphere();

private:
    void generateVertices(unsigned int stacks, unsigned int sectors, float radius);
};
