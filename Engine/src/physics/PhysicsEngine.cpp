#include "PhysicsEngine.h"
#include "renderer/PhysicsObject.h"

rp3d::PhysicsCommon PhysicsEngine::physicsCommon;
rp3d::PhysicsWorld* PhysicsEngine::physicsWorld;
std::vector<PhysicsObject*> PhysicsEngine::m_physicsObjects;

void PhysicsEngine::init(){
    physicsWorld = physicsCommon.createPhysicsWorld();
}

void PhysicsEngine::update(float deltaTime){
    physicsWorld->update(deltaTime);
    for(PhysicsObject* obj : m_physicsObjects){
        obj->update();
    }
}

void PhysicsEngine::registerBody(PhysicsObject* body){
    m_physicsObjects.push_back(body);
}

void PhysicsEngine::unregisterBody(PhysicsObject* body){
    m_physicsObjects.erase(std::remove(m_physicsObjects.begin(),m_physicsObjects.end(),body),m_physicsObjects.end());
}

rp3d::RigidBody* PhysicsEngine::createRigidBody(rp3d::Transform transform){
    return physicsWorld->createRigidBody(transform);
}

void PhysicsEngine::destroy(){
    physicsCommon.destroyPhysicsWorld(physicsWorld);
}