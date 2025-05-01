#pragma once
#include <Component/GuiComponent.h>
#include <Component/DisplayComponent.h>

namespace Client
{
	class LoginMenu :
		public GDE::GuiComponent
	{
	public:
		static constexpr auto type = "LoginMenu";

		LoginMenu(GDE::Entity& owner) : Component(owner) {}
		~LoginMenu() = default;
		LoginMenu(const LoginMenu&) = default;
		LoginMenu& operator=(const LoginMenu&) = default;
		LoginMenu(LoginMenu&& ActionGuiComponent) = default;
		LoginMenu& operator=(LoginMenu&&) = default;

		void setup(const GDE::ComponentDescription& init_value) override;

		void update(const GDE::Timing& dt) override;
	private:
		bool _loginError = false;
	};
}