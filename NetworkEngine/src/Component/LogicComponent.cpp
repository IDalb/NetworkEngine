#include "Component/LogicComponent.h"
#include "System/LogicSystem.h"

namespace GDE
{
	LogicComponent::LogicComponent()
	{
		LogicSystem::getInstance().registerComponent(this);
	}

	LogicComponent::~LogicComponent()
	{
		LogicSystem::getInstance().removeComponent(this);
	}
}