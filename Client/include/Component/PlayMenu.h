#pragma once
#include <Component/GuiComponent.h>
#include <Component/DisplayComponent.h>

namespace Client
{
	class PlayMenu :
		public GDE::GuiComponent
	{
	public:
		static constexpr auto type = "PlayMenu";

		PlayMenu(GDE::Entity& owner) : Component(owner) {}
		~PlayMenu() = default;
		PlayMenu(const PlayMenu&) = default;
		PlayMenu& operator=(const PlayMenu&) = default;
		PlayMenu(PlayMenu&& ActionGuiComponent) = default;
		PlayMenu& operator=(PlayMenu&&) = default;

		void setup(const GDE::ComponentDescription& init_value) override;

		void update(const GDE::Timing& dt) override;
	private:
		std::vector<std::string> _achievements;
		bool _waiting = false;
	};
}