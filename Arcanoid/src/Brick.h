#pragma once
#include <Engine.h>


class Brick : public PhysicsObject {
    public:
           Brick(const std::shared_ptr<Mesh>& mesh, rp3d::BodyType type = rp3d::BodyType::STATIC)
        : PhysicsObject(mesh, type),
          isBeingDestroyed(false),
          destructionTimer(0.0f)
    {
        Texture texture;
        texture.loadTexture("assets/textures/rene.jpg");
        this->m_Mesh->texture = texture;
    }

    public:
        bool isBeingDestroyed = false;
        float destructionTimer = 0.0f;
        void killBrick();
        void update()override;


};