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
        _bInertia = btVector3(0.0f, 0.0f, 0.0f);
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

            if (!Magnum::Math::TypeTraits<Magnum::Float>::equals(_mass, 0.0f))
                _bShape->calculateLocalInertia(_mass, _bInertia);

        /* Bullet rigidbody setup */
        _motionState = new Magnum::BulletIntegration::MotionState(owner().getComponent<TransformComponent>()->getTransform());
        _bRigidbody.emplace(btRigidBody::btRigidBodyConstructionInfo{
            _mass,
            &_motionState->btMotionState(),
            _bShape.get(),
            _bInertia
        });
        _bRigidbody->forceActivationState(DISABLE_DEACTIVATION);
        _bWorld->addRigidBody(_bRigidbody.get());
        _bRigidbody->setUserPointer(this);
        syncPose();

        shape->setSizeChangeCallback({ [](GDE::Component* component, const std::string& parameter) {
            if (auto rigidBody = dynamic_cast<GDE::RigidbodyComponent*>(component))
            {
                // Update the collision shape to match the new scale
                btCollisionShape* shape = rigidBody->getRigidBody().getCollisionShape();

                auto shapeComponent = rigidBody->owner().getComponent<GDE::ShapeComponent>();
                

                // Set the new shape
                switch (shapeComponent->_shape)
                {
                case  GDE::ShapeComponent::BOX:
                    rigidBody->setShapeToBox(shapeComponent->getData("sx"), shapeComponent->getData("sy"), shapeComponent->getData("sz"));
                    break;
                case  GDE::ShapeComponent::SPHERE:
                    if (auto* sphereShape = dynamic_cast<btSphereShape*>(shape))
                    {

                        sphereShape->setUnscaledRadius(shapeComponent->getData("radius"));
                    }
                    break;
                }
                
                rigidBody->syncPose();
            }
            }, this });
    }

    void RigidbodyComponent::updatePhysics(const Timing& timing)
    {
        syncPose();
    }

    void RigidbodyComponent::syncPose() {
        _bRigidbody->setWorldTransform(btTransform(owner().getComponent<TransformComponent>()->getTransform().absoluteTransformationMatrix()));
    }
    void RigidbodyComponent::setShapeToBox(float x, float y, float z)
    {
        _bWorld->removeRigidBody(_bRigidbody.get());
        _bShape = std::make_unique<btBoxShape>(btVector3(x, y, z));
        if (!Magnum::Math::TypeTraits<Magnum::Float>::equals(_mass, 0.0f))
            _bShape->calculateLocalInertia(_mass, _bInertia);
        _bRigidbody.reset();
        _bRigidbody.emplace(btRigidBody::btRigidBodyConstructionInfo{
            _mass,
                & _motionState->btMotionState(),
                _bShape.get(),
                _bInertia
        });
        _bRigidbody->setUserPointer(this);
        _bWorld->addRigidBody(_bRigidbody.get());
    }
}
