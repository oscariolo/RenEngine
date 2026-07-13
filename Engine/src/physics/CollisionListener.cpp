#include "CollisionListener.h"
#include "renderer/PhysicsObject.h"
#include <iostream>

void CollisionListener::onContact(const rp3d::CollisionCallback::CallbackData& callbackData) {
    for(unsigned int p = 0; p < callbackData.getNbContactPairs(); p++){
        rp3d::CollisionCallback::ContactPair contactPair = callbackData.getContactPair(p);
        void* userData1 = contactPair.getBody1()->getUserData();
        void* userData2 = contactPair.getBody2()->getUserData();

        if(!userData1 || !userData2) continue;

        PhysicsObject* obj1 = static_cast<PhysicsObject*>(userData1);
        PhysicsObject* obj2 = static_cast<PhysicsObject*>(userData2);

        if(obj1->m_isQueuedForDeletion || obj2->m_isQueuedForDeletion) continue;

        if(obj1->m_collisionCallback) obj1->m_collisionCallback(obj1, obj2);
        if(obj2->m_collisionCallback) obj2->m_collisionCallback(obj2, obj1);
    }
}

void CollisionListener::onTrigger(const rp3d::OverlapCallback::CallbackData& callbackData) {
    for(unsigned int p = 0; p < callbackData.getNbOverlappingPairs(); p++){
        rp3d::OverlapCallback::OverlapPair overlapPair = callbackData.getOverlappingPair(p);
        void* userData1 = overlapPair.getBody1()->getUserData();
        void* userData2 = overlapPair.getBody2()->getUserData();

        if(!userData1 || !userData2) continue;

        PhysicsObject* obj1 = static_cast<PhysicsObject*>(userData1);
        PhysicsObject* obj2 = static_cast<PhysicsObject*>(userData2);

        if(obj1->m_isQueuedForDeletion || obj2->m_isQueuedForDeletion) continue;

        if(obj1->m_collisionCallback) obj1->m_collisionCallback(obj1, obj2);
        if(obj2->m_collisionCallback) obj2->m_collisionCallback(obj2, obj1);
    }
}