#include "System/InterpolationSystem.h"
#include "Entity.h"
namespace GDE
{
	InterpolationSystem& InterpolationSystem::getInstance()
	{
		static InterpolationSystem logic_system;
		return logic_system;
	}

	void InterpolationSystem::iterate(const Timing& dt)
	{
		if(_enable)
		{
			for (auto& interpolation_component : _interpolationComponents)
			{
				if (interpolation_component->enabled() && interpolation_component->owner().active())
				{
					interpolation_component->interpolate(dt);
				}
			}
		}
	}
	void InterpolationSystem::registerComponent(InterpolationComponent* interpolation_component)
	{
		_interpolationComponents.insert(interpolation_component);
	}

	void InterpolationSystem::removeComponent(InterpolationComponent* interpolation_component)
	{
		_interpolationComponents.erase(interpolation_component);
	}
}
