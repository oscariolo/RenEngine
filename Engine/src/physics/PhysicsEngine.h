#pragma once
#include <reactphysics3d/reactphysics3d.h>
#include <vector>

class PhysicsObject;

class PhysicsEngine {
    public:
        static void destroy();
        static void init();
        static void update(float deltaTime);
        static rp3d::RigidBody* createRigidBody(rp3d::Transform transform);
        static void registerBody(PhysicsObject* body);
        static void unregisterBody(PhysicsObject* body);
        static void removeBody(PhysicsObject* body);

        static rp3d::PhysicsCommon physicsCommon;
        static rp3d::PhysicsWorld* physicsWorld;
        static std::vector<PhysicsObject*> m_physicsObjects; 
    


};