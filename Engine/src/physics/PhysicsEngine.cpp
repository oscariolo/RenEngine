#include "PhysicsEngine.h"
#include "renderer/PhysicsObject.h"
#include "CollisionListener.h"
#include <algorithm>

rp3d::PhysicsCommon PhysicsEngine::physicsCommon;
rp3d::PhysicsWorld* PhysicsEngine::physicsWorld;
CollisionListener collisionListener;
std::vector<PhysicsObject*> PhysicsEngine::m_physicsObjects;

void PhysicsEngine::init(){
    physicsWorld = physicsCommon.createPhysicsWorld();
    physicsWorld->setEventListener(&collisionListener);

}

void PhysicsEngine::update(float deltaTime){
    // Step 1: advance the simulation FIRST. Collision callbacks
    // (CollisionListener::onContact -> m_collisionCallback -> queue_free())
    // run synchronously inside this call, so any object hit this frame
    // gets flagged for deletion right here.
    physicsWorld->update(deltaTime);

    // Step 2: sync transforms for everything that's still alive.
    // Skip anything just flagged above - it's about to be destroyed below,
    // no point reading its transform.
    for(PhysicsObject* obj : m_physicsObjects){
        if(!obj->m_isQueuedForDeletion){
            obj->update();
        }
    }

    // Step 3: remove and actually destroy any bodies queued for deletion,
    // in this SAME frame, in a single erase-remove pass (never erase from
    // a container while range-for iterating it - that's UB). Doing this
    // in the same call that just flagged them (rather than waiting for
    // the top of the next call) matters: callers like main.cpp prune their
    // own shared_ptr<PhysicsObject> vectors right after this function
    // returns, and if PhysicsEngine hasn't removed its raw pointer to that
    // object yet, the next update() call will dereference freed memory.
    m_physicsObjects.erase(
        std::remove_if(m_physicsObjects.begin(), m_physicsObjects.end(),
            [](PhysicsObject* obj){
                if(obj->m_isQueuedForDeletion){
                    if(rp3d::RigidBody* body = obj->getRigidBody()){
                        physicsWorld->destroyRigidBody(body);
                        obj->setRigidBody(nullptr);
                    }
                    return true;
                }
                return false;
            }),
        m_physicsObjects.end());
}

void PhysicsEngine::registerBody(PhysicsObject* body){
    m_physicsObjects.push_back(body);
}

void PhysicsEngine::unregisterBody(PhysicsObject* body){
    if(rp3d::RigidBody* rigidBody = body->getRigidBody()){
        physicsWorld->destroyRigidBody(rigidBody);
        body->setRigidBody(nullptr);
    }
    m_physicsObjects.erase(std::remove(m_physicsObjects.begin(),m_physicsObjects.end(),body),m_physicsObjects.end());
}

rp3d::RigidBody* PhysicsEngine::createRigidBody(rp3d::Transform transform){
    return physicsWorld->createRigidBody(transform);
}

void PhysicsEngine::destroy(){
    physicsCommon.destroyPhysicsWorld(physicsWorld);
}