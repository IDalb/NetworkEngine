#pragma once
#include "Component/Component.h"
#include "Timing.h"
namespace GDE
{
	class GuiComponent
		: virtual public Component
	{
	public:

		GuiComponent();
		~GuiComponent();
		GuiComponent(const GuiComponent&) = default;
		GuiComponent& operator=(const GuiComponent&) = default;
		GuiComponent(GuiComponent&&) = default;
		GuiComponent& operator=(GuiComponent&&) = default;

		virtual void update(const Timing& dt) = 0;
	};
}