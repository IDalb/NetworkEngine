#include "Component/AlternateLogicComponent.h"
#include "System/AlternateLogicSystem.h"

namespace GDE
{
	AlternateLogicComponent::AlternateLogicComponent()
	{
		if(AlternateLogicSystem::_exist)
			AlternateLogicSystem::getInstance().registerComponent(this);
	}

	AlternateLogicComponent::~AlternateLogicComponent()
	{
		if(AlternateLogicSystem::_exist)
			AlternateLogicSystem::getInstance().removeComponent(this);
	}
}