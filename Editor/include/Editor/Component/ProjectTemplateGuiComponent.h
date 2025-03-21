#pragma once
#include <Component/GuiComponent.h>
#include <Component/AlternateLogicComponent.h>
#include <Component/DisplayComponent.h>

namespace GDEEditor
{
	class ProjectTemplateGuiComponent :
		public GDE::GuiComponent, public GDE::AlternateLogicComponent
	{
	public:
		static constexpr auto type = "ProjectTemplateGui";

		ProjectTemplateGuiComponent(GDE::Entity& owner) : Component(owner) {}
		~ProjectTemplateGuiComponent() = default;
		ProjectTemplateGuiComponent(const ProjectTemplateGuiComponent&) = default;
		ProjectTemplateGuiComponent& operator=(const ProjectTemplateGuiComponent&) = default;
		ProjectTemplateGuiComponent(ProjectTemplateGuiComponent&&) = default;
		ProjectTemplateGuiComponent& operator=(ProjectTemplateGuiComponent&&) = default;

		void setup(const GDE::ComponentDescription& init_value) override;

		void update(const GDE::Timing& dt) override;
		void updateAlternateLogic(const GDE::Timing& timing) override;
	private:
		bool _placeCube = false;
	};
}