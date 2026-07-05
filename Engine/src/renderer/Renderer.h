#pragma once
#include "Shader.h"
#include "Camera.h"
#include <memory>
#include "Mesh.h"
#include "GameObject.h"
#include <glm/glm.hpp>
#include "PointLight.h"

class Renderer {

    public:
        static void init();

        static void setCamera(Camera& camera);
        static void addPointLight(const std::shared_ptr<PointLight>& pointLight);
        //genera los buffers y los prepara para dibujar
        static void submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<GameObject>& gameObject); 

        //dibuja todo los objetos que se han enviado al renderer
        static void draw();

        static void Clear();
        
        static Camera* m_Camera;
        static glm::mat4 m_ProjectionMatrix;
        static std::vector<std::shared_ptr<PointLight>> m_PointLights;
        
    
};