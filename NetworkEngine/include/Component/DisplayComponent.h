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
		virtual ~DisplayComponent();
		DisplayComponent(const DisplayComponent&) = default;
		DisplayComponent& operator=(const DisplayComponent&) = default;
		DisplayComponent(DisplayComponent&&) = default;
		DisplayComponent& operator=(DisplayComponent&&) = default;

		virtual void display(Magnum::Shaders::PhongGL& shader, const Timing& dt) = 0;

	};
}