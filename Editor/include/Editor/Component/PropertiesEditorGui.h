#pragma once
#include <Component/GuiComponent.h>
#include <Component/DisplayComponent.h>
#include <Component/AlternateLogicComponent.h>

namespace GDEEditor
{
	class PropertiesEditorGuiComponent :
		public GDE::GuiComponent, public GDE::AlternateLogicComponent
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
		void updateAlternateLogic(const GDE::Timing& timing) override;

		void selectEntity(GDE::Entity* entity);
		void removeSelection() { _selectedEntity = nullptr; }

	private:
		GDE::Entity* _selectedEntity = nullptr;
		std::unordered_map<std::string, std::unordered_map<std::string, float>> _transformValue;
		std::unordered_map<std::string, std::unordered_map<std::string, float>> _shapeValue;
	};
}