#include "System/LogicSystem.h"
#include "Entity.h"
namespace GDE
{
	LogicSystem& LogicSystem::getInstance()
	{
		static LogicSystem logic_system;
		return logic_system;
	}

	void LogicSystem::iterate(const Timing& dt)
	{
		if(_enable)
		{
			for (auto& logic_component : _logicComponents)
			{
				if (logic_component->enabled() && logic_component->owner().active())
				{
					logic_component->updateLogic(dt);
				}
			}
		}
	}
	void LogicSystem::registerComponent(LogicComponent* logic_component)
	{
		_logicComponents.insert(logic_component);
	}

	void LogicSystem::removeComponent(LogicComponent* logic_component)
	{

		_logicComponents.erase(logic_component);
		
	}
}
