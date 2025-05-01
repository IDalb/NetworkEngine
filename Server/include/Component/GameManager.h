#pragma once
#include <Component/Component.h>

namespace Server
{
	class GameManager : public GDE::Component
	{
	public:
		static constexpr auto type = "GameManager";


		GameManager(GDE::Entity& parent) : GDE::Component(parent) {}
		void setup(const GDE::ComponentDescription& init_value) override {}

		void addPoint(uint32_t player);
	private:
		std::array<unsigned int, 4> _points{ 0,0,0,0 };
		
		void saveStats(uint32_t winner);
	};
}