#include "PhysicsObject.h"
#include "physics/PhysicsEngine.h"

PhysicsObject::PhysicsObject(const std::shared_ptr<Mesh>& mesh, rp3d::BodyType type) : GameObject(mesh)  {
    rp3d::Vector3 position(this->position.x, this->position.y, this->position.z);
    rp3d::Quaternion orientation = rp3d::Quaternion::identity();
    rp3d::Transform transform(position, orientation);
    m_RigidBody = PhysicsEngine::createRigidBody(transform);
    m_RigidBody->setUserData(this);
    m_RigidBody->setType(type);
};

PhysicsObject::PhysicsObject(rp3d::BodyType type) : GameObject() {
    rp3d::Vector3 position(this->position.x, this->position.y, this->position.z);
    rp3d::Quaternion orientation = rp3d::Quaternion::identity();
    rp3d::Transform transform(position, orientation);
    m_RigidBody = PhysicsEngine::createRigidBody(transform);
    m_RigidBody->setUserData(this);
    m_RigidBody->setType(type);
};


PhysicsObject::~PhysicsObject() {
    
}

glm::mat4 PhysicsObject::getTransform() const {
        // 1. Ask ReactPhysics3D exactly where the object is
        const rp3d::Transform& t = m_RigidBody->getTransform();

        // 2. Extract the OpenGL Matrix
        float matrix[16];
        t.getOpenGLMatrix(matrix);

        // 3. Convert it into a glm::mat4 (This contains Position and Rotation)
        glm::mat4 physicsTransform = glm::make_mat4(matrix);

        // 4. Apply the base GameObject's Scale!
        physicsTransform = glm::scale(physicsTransform, this->scale);

        return physicsTransform;
}

void PhysicsObject::setPosition(float x, float y, float z){
    rp3d::Transform currentTransform = m_RigidBody->getTransform();
    currentTransform.setPosition(rp3d::Vector3(x,y,z));
    m_RigidBody->setTransform(currentTransform);
    update();
}

void PhysicsObject::update(){
    rp3d::Transform currentTransform = m_RigidBody->getTransform();
    this->position.x = currentTransform.getPosition().x;
    this->position.y = currentTransform.getPosition().y;
    this->position.z = currentTransform.getPosition().z;
}

void PhysicsObject::setPosition(glm::vec3 position){
    rp3d::Transform currentTransform = m_RigidBody->getTransform();
    currentTransform.setPosition(rp3d::Vector3(position.x,position.y,position.z));
    m_RigidBody->setTransform(currentTransform);
}

void PhysicsObject::setRotation(float x, float y, float z){
    rp3d::Transform currentTransform = m_RigidBody->getTransform();
    rp3d::Quaternion rotation = rp3d::Quaternion::fromEulerAngles(rp3d::Vector3(x,y,z));
    currentTransform.setOrientation(rotation);
    m_RigidBody->setTransform(currentTransform);
}

void PhysicsObject::addCollider(rp3d::CollisionShape* collisionShape, int collisionGroup){
    rp3d::Transform transform = rp3d::Transform::identity();
    m_collider = m_RigidBody->addCollider(collisionShape,transform);
    m_collider->setCollisionCategoryBits(collisionGroup);
    m_collider->setCollideWithMaskBits(collisionGroup); // Collide with its collisionGroup only by default
}

void PhysicsObject::addCollisionLayer(int collisionGroup){
    if(m_collider){
        unsigned short currentMask = m_collider->getCollideWithMaskBits();
        currentMask |= collisionGroup; // Add the new group to the mask
        m_collider->setCollideWithMaskBits(currentMask);
    }
}

void PhysicsObject::setMaterialProperties(float bounciness, float frictionCoefficient, float massDensity){
    if(m_collider){
        rp3d::Material& material = m_collider->getMaterial();
        material.setBounciness(bounciness);
        material.setFrictionCoefficient(frictionCoefficient);
        material.setMassDensity(massDensity);
        m_RigidBody->setLinearDamping(0.0f); // Set linear damping to 0
        m_RigidBody->setAngularDamping(0.0f); // Set angular damping to 0
    }
}

void PhysicsObject::setRigidBody(rp3d::RigidBody* rigidBody) {
    m_RigidBody = rigidBody;
}

void PhysicsObject::queue_free() {
    m_isQueuedForDeletion = true;
}

