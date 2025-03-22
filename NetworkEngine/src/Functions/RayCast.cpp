#include "Functions/RayCast.h"

namespace GDE
{
	RaycastResult performRaycast(btDynamicsWorld* world, const Magnum::Vector3& start, const Magnum::Vector3& end)
	{
        btVector3 btStart(start.x(), start.y(), start.z());
        btVector3 btEnd(end.x(), end.y(), end.z());

        btCollisionWorld::ClosestRayResultCallback rayCallback(btStart, btEnd);
        world->rayTest(btStart, btEnd, rayCallback);

        if (rayCallback.hasHit())
        {
            // Remove const qualifier for upcasting
            btRigidBody* btBody = btRigidBody::upcast(const_cast<btCollisionObject*>(rayCallback.m_collisionObject));

            RigidbodyComponent* rigidBody = btBody ? static_cast<RigidbodyComponent*>(btBody->getUserPointer()) : nullptr;

            return {
                true,
                Magnum::Vector3(rayCallback.m_hitPointWorld.x(), rayCallback.m_hitPointWorld.y(), rayCallback.m_hitPointWorld.z()),
                rigidBody
            };
        }

        return { false, {}, nullptr };
	}
}