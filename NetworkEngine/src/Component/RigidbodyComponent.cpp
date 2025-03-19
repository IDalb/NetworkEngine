#include "Component/RigidbodyComponent.h"

#include "BulletDynamics/Dynamics/btRigidBody.h"
#include "System/PhysicsSystem.h"

GDE::RigidbodyComponent::~RigidbodyComponent() {
    _bWorld->removeRigidBody(_bRigidbody.get());
}

void GDE::RigidbodyComponent::setup(const ComponentDescription &init_value)
{
    _mass = init_value.parameters.contains("mass") ? init_value.parameters.at("mass").as<float>() : 1.0f;
}

void GDE::RigidbodyComponent::resolve()
{
    _bWorld = PhysicsSystem::getInstance().getWorld();

    btVector3 bInertia(0.0f, 0.0f, 0.0f);
    if (!Magnum::Math::TypeTraits<Magnum::Float>::equals(_mass, 0.0f))
        _bShape->calculateLocalInertia(_mass, bInertia);

    /* Bullet rigidbody setup */
    auto* motionState = new Magnum::BulletIntegration::MotionState{ *this };
    _bRigidbody.emplace(btRigidBody::btRigidBodyConstructionInfo{
        _mass,
        &motionState->btMotionState(),
        _bShape,
        bInertia
    });
    _bRigidbody->forceActivationState(DISABLE_DEACTIVATION);
    _bWorld->addRigidBody(_bRigidbody.get());
}

void GDE::RigidbodyComponent::syncPose() {
    _bRigidbody->setWorldTransform(btTransform(transformationMatrix()));
}
