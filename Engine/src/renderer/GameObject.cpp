#include "GameObject.h"
#include <glad/glad.h>

GameObject::GameObject(){
    m_transform = glm::mat4(1.0f);
}

GameObject::~GameObject() {
    
}

GameObject::GameObject(const std::shared_ptr<Mesh>& mesh) : m_Mesh(mesh) {

}

void GameObject::setMesh(const std::shared_ptr<Mesh>& mesh) {
    m_Mesh = mesh;

}


void GameObject::draw() {
    // Bind VAO and draw the mesh
    glBindVertexArray(m_Mesh->VAO_ID);
    glDrawElements(GL_TRIANGLES, m_Mesh->getIndices().size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}


