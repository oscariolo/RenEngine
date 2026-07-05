#include "Renderer.h"
#include "GameObject.h"
#include <glad/glad.h>
#include "utils/Projections.h"

//Renderer va a considerar la camara
//aplicar shaders a objetos que se determinen
//llamar a la funciones para dibujar una vez aplicado los shaders y transformaciones

Camera* Renderer::m_Camera = nullptr;
glm::mat4 Renderer::m_ProjectionMatrix = glm::mat4(1.0f);

void Renderer::init() {
    m_ProjectionMatrix = Projections::perspective();
}


void Renderer::setCamera(Camera& camera) {
    m_Camera = &camera;
}

void Renderer::submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<GameObject>& gameObject) {
    //Generate buffers and prepare them for drawing
    shader->Bind();

    shader->SetMat4("model", gameObject->getTransform());
    shader->SetMat4("view", m_Camera->getViewMatrix());
    shader->SetMat4("projection", m_ProjectionMatrix);

    gameObject->draw();    

}