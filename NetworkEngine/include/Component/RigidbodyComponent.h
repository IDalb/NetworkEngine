#pragma once
#include "Component.h"
#include "BulletCollision/CollisionShapes/btBoxShape.h"
#include "BulletDynamics/Dynamics/btDynamicsWorld.h"
#include "Corrade/Containers/Pointer.h"
#include "Magnum/BulletIntegration/MotionState.h"
#include "Magnum/Math/TypeTraits.h"

using namespace Magnum;

namespace GDE
{
    class RigidbodyComponent : public Component, public Object3D {
    private:
        Float _mass;
        btCollisionShape* _bShape;
        btDynamicsWorld& _bWorld;
        Containers::Pointer<btRigidBody> _bRigidbody;

    public:
        RigidbodyComponent(Entity& owner) : Component(owner) {
            _bShape = new btBoxShape(btVector3(1, 1, 1));
            _bWorld = nullptr;
        }


        RigidbodyComponent(Object3D* parent, Float mass, btCollisionShape* bShape, btDynamicsWorld& bWorld)
            : Object3D(parent), _bWorld(bWorld)
        {
            // Calculate inertia
            btVector3 bInertia(0.f, 0.f, 0.f);
            if (!Math::TypeTraits<Float>::equals(mass, 0.f))
                bShape->calculateLocalInertia(mass, bInertia);

            // Setup Bullet rigidbody
            auto* motionState = new BulletIntegration::MotionState{*this};
            _bRigidbody.emplace(btRigidBody::btRigidBodyConstructionInfo {
                mass, &motionState->btMotionState(), bShape, bInertia});
            _bRigidbody->forceActivationState(DISABLE_DEACTIVATION);
            bWorld.addRigidBody(_bRigidbody.get());
        }
        ~RigidbodyComponent() override;

        void setup(const ComponentDescription &init_value) override;
        void syncPose();

        btRigidBody& rigidBody() { return *_bRigidbody; }
    };
}
