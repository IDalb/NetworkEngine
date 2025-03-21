#include "Component/PhysicsComponent.h"
#include "System/PhysicsSystem.h"

namespace GDE
{
	PhysicsComponent::PhysicsComponent()
	{
		PhysicsSystem::getInstance().registerComponent(this);
	}

	PhysicsComponent::~PhysicsComponent()
	{
		PhysicsSystem::getInstance().removeComponent(this);
	}
}