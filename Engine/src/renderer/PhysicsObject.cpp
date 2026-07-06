#include "PhysicsObject.h"
#include "physics/PhysicsEngine.h"

PhysicsObject::PhysicsObject(const std::shared_ptr<Mesh>& mesh, rp3d::BodyType type) : GameObject(mesh) {
    rp3d::Vector3 position(this->position.x, this->position.y, this->position.z);
    rp3d::Quaternion orientation = rp3d::Quaternion::identity();
    rp3d::Transform transform(position, orientation);
    m_RigidBody = PhysicsEngine::createRigidBody(transform);
    PhysicsEngine::registerBody(this);
    m_RigidBody->setType(type);
};

PhysicsObject::~PhysicsObject() {
    PhysicsEngine::physicsWorld->destroyRigidBody(m_RigidBody);
    PhysicsEngine::unregisterBody(this);
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
}

void PhysicsObject::update(){
    rp3d::Transform currentTransform = m_RigidBody->getTransform();
    this->position.x = currentTransform.getPosition().x;
    this->position.y = currentTransform.getPosition().y;
    this->position.z = currentTransform.getPosition().z;
}

void PhysicsObject::addCollider(rp3d::CollisionShape* collisionShape){
    rp3d::Transform transform = rp3d::Transform::identity();
    m_collider = m_RigidBody->addCollider(collisionShape,transform);
}
