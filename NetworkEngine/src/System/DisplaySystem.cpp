#include "System/DisplaySystem.h"
#include "Component/DisplayComponent.h"

#include "Entity.h"
#include "GlobalConstants.h"

namespace GDE
{
	DisplaySystem& DisplaySystem::getInstance()
	{
		static DisplaySystem display_system;
		return display_system;
	}


	void DisplaySystem::iterate(const Timing& dt)
	{
		for (auto& display_component : display_components_)
		{
			if (display_component->enabled() && display_component->owner().active())
			{
				display_component->display(dt);
			}
		}
	}

	void DisplaySystem::registerComponent(DisplayComponent* display_component)
	{
		display_components_.insert(display_component);
	}
	void DisplaySystem::removeComponent(DisplayComponent* display_component)
	{
		display_components_.erase(display_component);
	}
}