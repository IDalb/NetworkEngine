#pragma once
#include "Component.h"
#include <Magnum/BulletIntegration/Integration.h>
#include <Magnum/BulletIntegration/MotionState.h>

#include <btBulletDynamicsCommon.h>
#include "Corrade/Containers/Pointer.h"
#include "Magnum/BulletIntegration/MotionState.h"

namespace GDE
{
    class RigidbodyComponent : public Component {
    private:
        float _mass;
        btCollisionShape* _bShape;
        btDynamicsWorld* _bWorld;
        Magnum::Containers::Pointer<btRigidBody> _bRigidbody;

    public:
        static constexpr auto type = "RigidBody";

        RigidbodyComponent(Entity& owner) : Component(owner) {}
        ~RigidbodyComponent();

        void setup(const ComponentDescription &init_value) override;
        void resolve() override;
        void syncPose();

        btRigidBody& getRigidBody() { return *_bRigidbody; }
    };
}
