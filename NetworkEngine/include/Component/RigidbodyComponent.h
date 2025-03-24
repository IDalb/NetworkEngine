#pragma once
#include "Component/PhysicsComponent.h"
#include <Magnum/BulletIntegration/Integration.h>
#include <Magnum/BulletIntegration/MotionState.h>

#include <btBulletDynamicsCommon.h>
#include "TransformComponent.h"
#include "Corrade/Containers/Pointer.h"
#include "Magnum/BulletIntegration/MotionState.h"

namespace GDE
{
    class RigidbodyComponent :public PhysicsComponent
    {
    private:
        float _mass;
        std::unique_ptr<btCollisionShape> _bShape;
        btDynamicsWorld* _bWorld;
        TransformComponent* _transform;
        Magnum::Containers::Pointer<btRigidBody> _bRigidbody;
        Magnum::BulletIntegration::MotionState* _motionState;

        btVector3 _bInertia;

    public:
        void syncPose();
        static constexpr auto type = "RigidBody";

        RigidbodyComponent(Entity& owner) : Component(owner) {}
        ~RigidbodyComponent();

        void updatePhysics(const Timing& timing) override;
        void setup(const ComponentDescription &init_value) override;
        void resolve() override;

        btRigidBody& getRigidBody() { return *_bRigidbody; }
        float getMass() { return _mass; }
        void setMass(float mass) { _mass = mass; _bRigidbody->setMassProps(_mass, _bInertia); }
        void setValue(std::string_view variable, float value) override;
        void setShapeToBox(float x, float y, float z);
    };
}
