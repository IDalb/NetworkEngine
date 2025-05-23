#pragma once
#include <btBulletDynamicsCommon.h>

#include "Magnum/BulletIntegration/DebugDraw.h"
#include "BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h"
#include "BulletCollision/BroadphaseCollision/btDbvtBroadphase.h"
#include "BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h"
#include "BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h"
#include "Component/PhysicsComponent.h"
#include "System/System.h"

namespace GDE
{
    class PhysicsSystem : public System {


    private:
        bool _enable = true;      
        std::unordered_set<PhysicsComponent*> _physicsComponent;

        std::unique_ptr<btDiscreteDynamicsWorld> _bWorld;
        //std::unique_ptr<Magnum::BulletIntegration::DebugDraw> _debugDraw;

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
            //_debugDraw.reset();
        }
        static PhysicsSystem& getInstance();

        //Magnum::BulletIntegration::DebugDraw* getDebugDraw() { return _debugDraw.get(); }
        btDiscreteDynamicsWorld* getWorld() const { return _bWorld.get(); };

        void registerComponent(PhysicsComponent* physics_component);
        void removeComponent(PhysicsComponent* physics_component);

        void setEnable(bool enable) { _enable = enable; }
    };
}
