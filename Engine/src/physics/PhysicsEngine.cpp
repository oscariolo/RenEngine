#include "PhysicsEngine.h"
#include "renderer/PhysicsObject.h"
#include "CollisionListener.h"
#include <algorithm>

rp3d::PhysicsCommon PhysicsEngine::physicsCommon;
rp3d::PhysicsWorld* PhysicsEngine::physicsWorld;
CollisionListener collisionListener;
std::vector<std::unique_ptr<PhysicsObject>> PhysicsEngine::m_physicsObjects;
// PhysicsEngine.h
std::function<void(PhysicsObject*)> PhysicsEngine::onBeforeDestroy;


void PhysicsEngine::init(){
    physicsWorld = physicsCommon.createPhysicsWorld();
    physicsWorld->setEventListener(&collisionListener);

}

void PhysicsEngine::update(float deltaTime){
    physicsWorld->update(deltaTime);

    // Sync transforms and free flagged objects in one pass.
    // remove_if keeps stable order and only calls unique_ptr's destructor
    // (freeing the object) for elements that return true — i.e. dead ones.
    m_physicsObjects.erase(
        std::remove_if(m_physicsObjects.begin(), m_physicsObjects.end(),
            [](const std::unique_ptr<PhysicsObject>& obj){
                if(obj->m_isQueuedForDeletion){
                    if(onBeforeDestroy) onBeforeDestroy(obj.get());
                    if(rp3d::RigidBody* body = obj->getRigidBody()){
                        physicsWorld->destroyRigidBody(body);
                        obj->setRigidBody(nullptr);
                    }
                    return true;
                }
                obj->update();
                return false;
            }),
        m_physicsObjects.end());
}

void PhysicsEngine::registerBody(PhysicsObject* body){
    m_physicsObjects.push_back(std::unique_ptr<PhysicsObject>(body));
}

void PhysicsEngine::unregisterBody(PhysicsObject* body){
    m_physicsObjects.erase(
        std::remove_if(m_physicsObjects.begin(), m_physicsObjects.end(),
            [body](const std::unique_ptr<PhysicsObject>& obj){ return obj.get() == body; }),
        m_physicsObjects.end());
}

rp3d::RigidBody* PhysicsEngine::createRigidBody(rp3d::Transform transform){
    return physicsWorld->createRigidBody(transform);
}

void PhysicsEngine::destroy(){
    physicsCommon.destroyPhysicsWorld(physicsWorld);
}