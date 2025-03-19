#include "Component/GuiComponent.h"
#include "System/GuiSystem.h"

namespace GDE
{
	GuiComponent::GuiComponent()
	{
		GuiSystem::getInstance().registerComponent(this);
	}
	GuiComponent::~GuiComponent()
	{
		GuiSystem::getInstance().removeComponent(this);
	}
}