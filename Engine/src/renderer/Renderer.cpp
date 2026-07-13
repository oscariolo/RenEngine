#include "Renderer.h"
#include "GameObject.h"
#include <glad/glad.h>
#include "utils/Projections.h"

//Renderer va a considerar la camara
//aplicar shaders a objetos que se determinen
//llamar a la funciones para dibujar una vez aplicado los shaders y transformaciones

Camera* Renderer::m_Camera = nullptr;
glm::mat4 Renderer::m_ProjectionMatrix = glm::mat4(1.0f);
std::vector<std::shared_ptr<PointLight>> Renderer::m_PointLights;

void Renderer::init() {
    m_ProjectionMatrix = Projections::perspective();
}


void Renderer::setCamera(Camera& camera) {
    m_Camera = &camera;
}

void Renderer::addPointLight(const std::shared_ptr<PointLight>& pointLight) {
    m_PointLights.push_back(pointLight);
}

void Renderer::submit(Shader* shader, GameObject* gameObject) {
    if(!shader || !gameObject || !gameObject->m_Mesh) {
        return;
    }
    //Generate buffers and prepare them for drawing
    shader->Bind();

    shader->SetMat4("model", gameObject->getTransform());
    shader->SetMat4("view", m_Camera->getViewMatrix());
    shader->SetVec3("cameraPos", m_Camera->getPosition());
    shader->SetMat4("projection", m_ProjectionMatrix);

    if(m_PointLights.size() > 0){
        for(size_t i = 0; i < m_PointLights.size(); ++i){
            std::string indexStr = std::to_string(i);
            shader->SetVec3("pointLights[" + indexStr + "].position", m_PointLights[i]->position);
            shader->SetVec3("pointLights[" + indexStr + "].ambient", m_PointLights[i]->ambient);
            shader->SetVec3("pointLights[" + indexStr + "].diffuse", m_PointLights[i]->diffuse);
            shader->SetVec3("pointLights[" + indexStr + "].specular", m_PointLights[i]->specular);
        }
        shader->SetInt("numLights", static_cast<int>(m_PointLights.size()));
    }
    //Texturas 
    if(gameObject->texture && gameObject->texture->m_textureID != 0){
        shader->SetInt("useTexture", 1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gameObject->texture->m_textureID);
    }else{
        shader->SetInt("useTexture", 0);
    }
    glBindVertexArray(gameObject->m_Mesh->VAO_ID);
    glDrawElements(GL_TRIANGLES, gameObject->m_Mesh->getIndices().size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0); 

}

void Renderer::updateProjection(int width, int height) {
    if(height == 0) return;
    float aspect = static_cast<float>(width) / static_cast<float>(height);
    m_ProjectionMatrix = Projections::perspective(aspect);
}

