#pragma once
#include <Component/GuiComponent.h>
#include <Component/DisplayComponent.h>

namespace GDEEditor
{
	class PropertiesEditorGuiComponent :
		public GDE::GuiComponent
	{
	public:
		static constexpr auto type = "PropertiesEditorGui";

		PropertiesEditorGuiComponent(GDE::Entity& owner) : Component(owner) {}
		~PropertiesEditorGuiComponent() = default;
		PropertiesEditorGuiComponent(const PropertiesEditorGuiComponent&) = default;
		PropertiesEditorGuiComponent& operator=(const PropertiesEditorGuiComponent&) = default;
		PropertiesEditorGuiComponent(PropertiesEditorGuiComponent&&) = default;
		PropertiesEditorGuiComponent& operator=(PropertiesEditorGuiComponent&&) = default;

		void setup(const GDE::ComponentDescription& init_value) override;

		void update(const GDE::Timing& dt) override;

	private:

	};
}