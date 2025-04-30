#include "Component/InterpolationComponent.h"
#include "System/InterpolationSystem.h"

namespace GDE
{
	InterpolationComponent::InterpolationComponent()
	{
		InterpolationSystem::getInstance().registerComponent(this);
	}

	InterpolationComponent::~InterpolationComponent()
	{
		InterpolationSystem::getInstance().removeComponent(this);
	}
}