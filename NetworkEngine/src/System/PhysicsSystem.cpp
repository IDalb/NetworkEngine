#include "System/PhysicsSystem.h"

namespace GDE
{
    PhysicsSystem::PhysicsSystem()
    {
        // Debug Draw
        _debugDraw = std::make_unique<Magnum::BulletIntegration::DebugDraw>();
        _debugDraw->setMode(Magnum::BulletIntegration::DebugDraw::Mode::DrawWireframe);

        _bWorld = std::make_unique<btDiscreteDynamicsWorld>(&_bDispatcher, &_bBroadphase, &_bSolver, &_bCollisionConfig);

        _bWorld->setGravity({ 0.0f, 0.f, -10.0f });
        _bWorld->setDebugDrawer(_debugDraw.get());
    }

    PhysicsSystem& PhysicsSystem::getInstance()
    {
        static PhysicsSystem display_system;
        return display_system;
    }

    void PhysicsSystem::iterate(const Timing& dt)
    {
        if (_enable)
        {
            _bWorld->stepSimulation(dt._dt, 5);
        }
    }
}
