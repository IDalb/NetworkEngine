#pragma once
#include "Component/Component.h"
#include "Timing.h"
namespace GDE
{
	class DisplayComponent
		: virtual public Component
	{
	public:

		DisplayComponent();
		~DisplayComponent();
		DisplayComponent(const DisplayComponent&) = default;
		DisplayComponent& operator=(const DisplayComponent&) = default;
		DisplayComponent(DisplayComponent&&) = default;
		DisplayComponent& operator=(DisplayComponent&&) = default;

		virtual void display(const Timing& dt) = 0;
	};
}