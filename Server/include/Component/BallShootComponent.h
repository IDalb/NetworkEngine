#pragma once
#include <Component/LogicComponent.h>

namespace Server
{
	class BallShootComponent : public GDE::LogicComponent
	{
	public:
		static constexpr auto type = "BallShoot";


		BallShootComponent(GDE::Entity& parent) : GDE::Component(parent) {}
		void setup(const GDE::ComponentDescription& init_value) override {}
		void updateLogic(const GDE::Timing& timing);

	private:
	};
}