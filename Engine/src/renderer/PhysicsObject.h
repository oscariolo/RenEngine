#pragma once
#include "GameObject.h"
#include "reactphysics3d/reactphysics3d.h"
#include <functional>

enum Category {
    CATEGORY1 = 0x0001,
    CATEGORY2 = 0x0002,
    CATEGORY3 = 0x0004,
};

class PhysicsObject : public GameObject{
    private:
        rp3d::RigidBody* m_RigidBody;
        rp3d::Collider* m_collider;

    
    public:
        std::function<void(PhysicsObject*, PhysicsObject*)> m_collisionCallback;    

        PhysicsObject(const std::shared_ptr<Mesh>& mesh, rp3d::BodyType type = rp3d::BodyType::STATIC);
        PhysicsObject(rp3d::BodyType type = rp3d::BodyType::DYNAMIC);
        virtual ~PhysicsObject(); //para destructores en objetos que heredan ej. luces

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
        void addCollider(rp3d::CollisionShape* collider, int collisionGroup = CATEGORY1);
        void addCollisionLayer(int collisionGroup);
        void queue_free();
        rp3d::Collider* getCollider() const {
            return m_collider;
        }

        bool m_isQueuedForDeletion = false;

};