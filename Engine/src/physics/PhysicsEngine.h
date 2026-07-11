#pragma once
#include <reactphysics3d/reactphysics3d.h>
#include <vector>

class PhysicsObject;

class PhysicsEngine {
    public:
        template<typename T, typename... Args>
        static T* spawn(Args&&... args) {
            auto obj = std::make_unique<T>(std::forward<Args>(args)...);
            T* ptr = obj.get();
            m_physicsObjects.push_back(std::move(obj));
            return ptr;
        }
        static void destroy();
        static void init();
        static void update(float deltaTime);
        static rp3d::RigidBody* createRigidBody(rp3d::Transform transform);
        static void registerBody(PhysicsObject* body);
        static void unregisterBody(PhysicsObject* body);
        static void removeBody(PhysicsObject* body);

        static rp3d::PhysicsCommon physicsCommon;
        static rp3d::PhysicsWorld* physicsWorld;
        static std::vector<std::unique_ptr<PhysicsObject>> m_physicsObjects;
        static std::function<void(PhysicsObject*)> onBeforeDestroy;


};