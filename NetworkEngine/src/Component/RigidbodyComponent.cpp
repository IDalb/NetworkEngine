#include "Component/RigidbodyComponent.h"

#include "BulletDynamics/Dynamics/btRigidBody.h"

GDE::RigidbodyComponent::~RigidbodyComponent() override {
    _bWorld.removeRigidBody(_bRigidbody.get());
}

void GDE::RigidbodyComponent::setup(const ComponentDescription &init_value) {

}

void GDE::RigidbodyComponent::syncPose() {
    _bRigidbody->setWorldTransform(btTransform(transformationMatrix()));
}
