#pragma once
#include <Magnum/Math/Vector3.h>
#include <btBulletDynamicsCommon.h>
#include "Component/RigidbodyComponent.h"
namespace GDE
{
    struct RaycastResult
    {
        bool hit;
        Magnum::Vector3 hitPoint;
        RigidbodyComponent* body;
    };

    RaycastResult performRaycast(btDynamicsWorld* world, const Magnum::Vector3& start, const Magnum::Vector3& end);
}