#pragma once

#include "BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h"
#include "System/System.h"

namespace GDE
{
    class PhysicsSystem : public System {
    private:
        btDiscreteDynamicsWorld *_bWorld;
    public:
        PhysicsSystem();
        ~PhysicsSystem() = default;
        void iterate(const Timing &dt) override;

        static PhysicsSystem& getInstance();

        btDiscreteDynamicsWorld* getWorld() const { return _bWorld; };
    };
}
