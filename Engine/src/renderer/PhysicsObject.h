#pragma once
#include "GameObject.h"
#include "reactphysics3d/reactphysics3d.h"
#include <functional>

class PhysicsObject : public GameObject{
    private:
        rp3d::RigidBody* m_RigidBody;
        rp3d::Collider* m_collider;

    
    public:
        std::function<void(PhysicsObject*, PhysicsObject*)> m_collisionCallback;    

        PhysicsObject(const std::shared_ptr<Mesh>& mesh, rp3d::BodyType type = rp3d::BodyType::STATIC);
        ~PhysicsObject();

        void setRigidBody(rp3d::RigidBody* rigidBody);
        void update()override;
        void setPosition(float x , float y, float z)override;
        void setPosition(glm::vec3 position)override;
        void setRotation(float x, float y, float z)override;
        void setMaterialProperties(float bounciness, float frictionCoefficient, float massDensity);
        void onCollisionCallback(std::function<void(PhysicsObject*, PhysicsObject*)> callback) {
            m_collisionCallback = callback;
        }


        rp3d::RigidBody* getRigidBody() const{
            return m_RigidBody;
        };
        glm::mat4 getTransform() const override;
        void addCollider(rp3d::CollisionShape* collider);
        void queue_free();

        bool m_isQueuedForDeletion = false;

};