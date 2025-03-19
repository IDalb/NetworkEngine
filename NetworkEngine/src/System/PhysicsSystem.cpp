#include "System/PhysicsSystem.h"

#include "BulletCollision/BroadphaseCollision/btDbvtBroadphase.h"
#include "BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h"
#include "BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h"
#include "Magnum/BulletIntegration/DebugDraw.h"

PhysicsSystem::PhysicsSystem() {
    // Debug Draw
    Magnum::BulletIntegration::DebugDraw _debugDraw{Corrade::NoCreate};
    _debugDraw = Magnum::BulletIntegration::DebugDraw{};
    _debugDraw.setMode(Magnum::BulletIntegration::DebugDraw::Mode::DrawWireframe);

    // Bullet world initialization
    btDbvtBroadphase _bBroadphase;
    btDefaultCollisionConfiguration _bCollisionConfig;
    btCollisionDispatcher _bDispatcher{ &_bCollisionConfig };
    btSequentialImpulseConstraintSolver _bSolver;

    _bWorld = new btDiscreteDynamicsWorld(&_bDispatcher, &_bBroadphase, &_bSolver, &_bCollisionConfig);

    _bWorld->setGravity({ 0.0f, -10.0f, 0.0f });
    _bWorld->setDebugDrawer(&_debugDraw);
}

PhysicsSystem& PhysicsSystem::getInstance()
{
    static PhysicsSystem display_system;
    return display_system;
}

void PhysicsSystem::iterate(const Timing &dt) {
    _bWorld->stepSimulation(dt._dt, 5);
}
