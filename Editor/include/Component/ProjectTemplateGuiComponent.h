#pragma once
#include <Component/GuiComponent.h>
#include <Component/DisplayComponent.h>

namespace GDEEditor
{
	class ProjectTemplateGuiComponent :
		public GDE::GuiComponent
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

	private:

	};
}