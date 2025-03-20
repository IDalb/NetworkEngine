#pragma once
#include "Component.h"
#include <Magnum/BulletIntegration/Integration.h>
#include <Magnum/BulletIntegration/MotionState.h>

#include <btBulletDynamicsCommon.h>
#include "TransformComponent.h"
#include "Corrade/Containers/Pointer.h"
#include "Magnum/BulletIntegration/MotionState.h"

namespace GDE
{
    class RigidbodyComponent : public Component {
    private:
        float _mass;
        std::unique_ptr<btCollisionShape> _bShape;
        btDynamicsWorld* _bWorld;
        TransformComponent* _transform;
        Magnum::Containers::Pointer<btRigidBody> _bRigidbody;
        Magnum::BulletIntegration::MotionState* _motionState;
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
