#include "Component/RigidbodyComponent.h"

#include "BulletDynamics/Dynamics/btRigidBody.h"

GDE::RigidbodyComponent::~RigidbodyComponent() {
    _bWorld->removeRigidBody(_bRigidbody.get());
}

void GDE::RigidbodyComponent::setup(const ComponentDescription &init_value)
{
    _mass = init_value.parameters.at("mass").as<float>();
}

void GDE::RigidbodyComponent::resolve()
{
}

void GDE::RigidbodyComponent::syncPose() {
    _bRigidbody->setWorldTransform(btTransform(transformationMatrix()));
}
