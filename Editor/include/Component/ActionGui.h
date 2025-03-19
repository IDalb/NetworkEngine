#pragma once
#include <Component/GuiComponent.h>
#include <Component/DisplayComponent.h>

namespace GDEEditor
{
	class ActionGuiComponent :
		public GDE::GuiComponent
	{
	public:
		static constexpr auto type = "ActionGui";

		ActionGuiComponent(GDE::Entity& owner) : Component(owner) {}
		~ActionGuiComponent() = default;
		ActionGuiComponent(const ActionGuiComponent&) = default;
		ActionGuiComponent& operator=(const ActionGuiComponent&) = default;
		ActionGuiComponent(ActionGuiComponent&& ActionGuiComponent) = default;
		ActionGuiComponent& operator=(ActionGuiComponent&&) = default;

		void setup(const GDE::ComponentDescription& init_value) override;

		void update(const GDE::Timing& dt) override;

	private:

	};
}