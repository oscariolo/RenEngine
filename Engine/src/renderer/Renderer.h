#pragma once
#include "Shader.h"
#include "Camera.h"
#include <memory>
#include <Mesh.h>

class Renderer {

    public:
        Renderer();
        ~Renderer();

        static void setCamera(Camera& camera);
        //genera los buffers y los prepara para dibujar
        static void submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<Mesh>& mesh, const glm::mat4& transform); 
        

        //dibuja todo los objetos que se han enviado al renderer
        static void draw();

        static void Clear();
    
    private:
        
    
};