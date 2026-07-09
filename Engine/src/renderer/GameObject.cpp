#include "GameObject.h"
#include "physics/PhysicsEngine.h"


GameObject::GameObject(){
    m_ID = generateUniqueID();
}

GameObject::~GameObject() {
    
}

GameObject::GameObject(const std::shared_ptr<Mesh>& mesh) : m_Mesh(mesh) {
    m_ID = generateUniqueID();
}

void GameObject::setMesh(const std::shared_ptr<Mesh>& mesh) {   
    m_Mesh = mesh;

}

glm::mat4 GameObject::getTransform() const { 
    glm::mat4 transform = glm::mat4(1.0f); // Start with identity matrix
    
    // 1. Translate
    transform = glm::translate(transform, position);
    
    // 2. Rotate (Convert degrees to radians for GLM)
    transform = glm::rotate(transform, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    transform = glm::rotate(transform, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    transform = glm::rotate(transform, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    
    // 3. Scale
    transform = glm::scale(transform, scale);

    
    return transform; 
};

void GameObject::setPosition(float x, float y, float z){
    position.x = x;
    position.y = y;
    position.z = z;
}


void GameObject::update(){

}

//solo dibuja el objeto, no actualiza nada
void GameObject::draw() {

}


