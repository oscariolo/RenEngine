#pragma once

#include "Mesh.h"
#include <memory>
#include <glm/glm.hpp>
#include <reactphysics3d/reactphysics3d.h>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 

class GameObject {
    public:
        GameObject();
        ~GameObject();

        GameObject(const std::shared_ptr<Mesh>& mesh);

        void setMesh(const std::shared_ptr<Mesh>& mesh);
        void setTransform(const glm::mat4& transform);
        virtual void setPosition(float x, float y, float z);
        virtual void setPosition(glm::vec3 position){
            this->position = position;
        }
        
        virtual void setRotation(float x, float y, float z) {
            rotation = glm::vec3(x, y, z);
        }
        
        virtual glm::vec3 getPosition() const {
            return position;
        }

        virtual glm::mat4 getTransform() const;
        

        std::shared_ptr<Mesh> getMesh() const;
        void draw();
        virtual void update();
        
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
        
    public:
        std::shared_ptr<Mesh> m_Mesh;
        glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
    



};

