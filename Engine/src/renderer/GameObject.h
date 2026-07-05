#pragma once

#include "Mesh.h"
#include <memory>
#include <glm/glm.hpp>

class VertexBuffer{
    public:
        unsigned int ID;
};


class GameObject {
    public:
        GameObject();
        ~GameObject();

        GameObject(const std::shared_ptr<Mesh>& mesh);

        void setMesh(const std::shared_ptr<Mesh>& mesh);
        void setTransform(const glm::mat4& transform);
        glm::mat4 getTransform() const;

        std::shared_ptr<Mesh> getMesh() const;
        void draw();

        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
        
    
    private:
        std::shared_ptr<Mesh> m_Mesh;
        glm::mat4 m_transform = glm::mat4(1.0f);




};

