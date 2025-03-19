#pragma once
#include "Component.h"
#include "BulletCollision/CollisionShapes/btBoxShape.h"
#include "BulletDynamics/Dynamics/btDynamicsWorld.h"
#include "Corrade/Containers/Pointer.h"
#include "Magnum/BulletIntegration/MotionState.h"
#include "Magnum/Math/TypeTraits.h"

namespace GDE
{
    class RigidbodyComponent : public Component, public Object3D {
    private:
        float _mass;
        btCollisionShape* _bShape;
        btDynamicsWorld* _bWorld;
        Magnum::Containers::Pointer<btRigidBody> _bRigidbody;

    public:
        RigidbodyComponent(Entity& owner) : Component(owner) {}
        ~RigidbodyComponent();

        void setup(const ComponentDescription &init_value) override;
        void resolve() override;
        void syncPose();

        btRigidBody& getRigidBody() { return *_bRigidbody; }
    };
}
