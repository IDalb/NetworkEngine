#pragma once
#include <btBulletDynamicsCommon.h>

#include "Magnum/BulletIntegration/DebugDraw.h"
#include "BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h"
#include "BulletCollision/BroadphaseCollision/btDbvtBroadphase.h"
#include "BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h"
#include "BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h"

#include "System/System.h"

namespace GDE
{
    class PhysicsSystem : public System {
    private:
        std::unique_ptr<btDiscreteDynamicsWorld> _bWorld;
        std::unique_ptr<Magnum::BulletIntegration::DebugDraw> _debugDraw;

        // Bullet world initialization
        btDbvtBroadphase _bBroadphase;
        btDefaultCollisionConfiguration _bCollisionConfig;
        btCollisionDispatcher _bDispatcher{ &_bCollisionConfig };
        btSequentialImpulseConstraintSolver _bSolver;
    public:
        PhysicsSystem();
        ~PhysicsSystem() = default;
        void iterate(const Timing &dt) override;
        void clean()
        {
            _debugDraw.reset();
        }
        static PhysicsSystem& getInstance();

        btDiscreteDynamicsWorld* getWorld() const { return _bWorld.get(); };
    };
}
