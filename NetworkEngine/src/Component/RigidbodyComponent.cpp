#include "Component/RigidbodyComponent.h"

#include "System/PhysicsSystem.h"

#include "Entity.h"
#include "Component/TransformComponent.h"

#include "Component/ShapeComponent.h"

namespace GDE
{
    RigidbodyComponent::~RigidbodyComponent() {
        _bWorld->removeRigidBody(_bRigidbody.get());
    }

    void RigidbodyComponent::setup(const ComponentDescription &init_value)
    {
        _mass = init_value.parameters.contains("mass") ? init_value.parameters.at("mass").as<float>() : 1.0f;
    }

    void RigidbodyComponent::resolve()
    {
        _transform = owner().getComponent<TransformComponent>();
        _bWorld = PhysicsSystem::getInstance().getWorld();

    auto shape = owner().getComponent<ShapeComponent>();
    switch (shape->_shape)
    {
    case ShapeComponent::BOX:
        _bShape = std::make_unique<btBoxShape>(btVector3(shape->getData("sx"), shape->getData("sy") , shape->getData("sz")));
        break;
    case ShapeComponent::SPHERE:
        _bShape = std::make_unique<btSphereShape>(shape->getData("radius"));
        break;
    }

        btVector3 bInertia(0.0f, 0.0f, 0.0f);
        if (!Magnum::Math::TypeTraits<Magnum::Float>::equals(_mass, 0.0f))
            _bShape->calculateLocalInertia(_mass, bInertia);

    /* Bullet rigidbody setup */
    auto* motionState = new Magnum::BulletIntegration::MotionState{ owner().getComponent<TransformComponent>()->getTransform() };
    _bRigidbody.emplace(btRigidBody::btRigidBodyConstructionInfo{
        _mass,
        &motionState->btMotionState(),
        _bShape.get(),
        bInertia
    });
    _bRigidbody->forceActivationState(DISABLE_DEACTIVATION);
    _bWorld->addRigidBody(_bRigidbody.get());
}

    void RigidbodyComponent::syncPose() {
        _bRigidbody->setWorldTransform(btTransform(owner().getComponent<TransformComponent>()->getTransform().transformationMatrix()));
    }
}
