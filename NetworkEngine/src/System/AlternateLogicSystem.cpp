#include "System/AlternateLogicSystem.h"
#include "Entity.h"
namespace GDE
{

	AlternateLogicSystem& AlternateLogicSystem::getInstance()
	{
		static AlternateLogicSystem logic_system;
		return logic_system;
	}

	void AlternateLogicSystem::iterate(const Timing& dt)
	{
		if (_enable)
		{
			for (auto& logic_component : _logicComponents)
			{
				if (logic_component->enabled() && logic_component->owner().active())
				{
					logic_component->updateAlternateLogic(dt);
				}
			}
		}
	}
	void AlternateLogicSystem::registerComponent(AlternateLogicComponent* logic_component)
	{
		_logicComponents.insert(logic_component);
	}

	void AlternateLogicSystem::removeComponent(AlternateLogicComponent* logic_component)
	{
		_logicComponents.erase(logic_component);
	}
}
