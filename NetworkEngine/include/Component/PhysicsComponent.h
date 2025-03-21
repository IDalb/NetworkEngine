#pragma once
#include "Component/Component.h"
#include "Timing.h"

namespace GDE
{
    class PhysicsComponent :
        public virtual Component
    {
    public :
        PhysicsComponent();
        ~PhysicsComponent();
        PhysicsComponent(const PhysicsComponent&) = default;
        PhysicsComponent& operator=(const PhysicsComponent&) = default;
        PhysicsComponent(PhysicsComponent&&) = default;
        PhysicsComponent& operator=(PhysicsComponent&&) = default;

        virtual void updatePhysics(const Timing& timing) = 0;
    };
}

