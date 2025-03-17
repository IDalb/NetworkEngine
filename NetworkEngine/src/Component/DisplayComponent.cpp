#include "Component/DisplayComponent.h"
#include "System/DisplaySystem.h"

namespace GDE
{
	DisplayComponent::DisplayComponent()
	{
		DisplaySystem::getInstance().registerComponent(this);
	}
	DisplayComponent::~DisplayComponent()
	{
		DisplaySystem::getInstance().removeComponent(this);
	}
}