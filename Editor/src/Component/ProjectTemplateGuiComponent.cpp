#include "Component/ProjectTemplateGuiComponent.h"
#include <Game.h>
#include <imgui.h>
namespace GDEEditor
{
	void ProjectTemplateGuiComponent::setup(const GDE::ComponentDescription& init_value)
	{

	}

	void ProjectTemplateGuiComponent::update(const GDE::Timing& dt)
	{
		ImGuiIO& io = ImGui::GetIO();  // Store a reference
		ImVec2 screenSize = io.DisplaySize;
		ImGui::SetNextWindowPos(ImVec2(screenSize.x * 0.15, screenSize.y * 0.8));
		ImGui::SetNextWindowSize(ImVec2(screenSize.x * 0.7, screenSize.y * 0.2));
		ImGui::Begin("Template drawer", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
		ImGui::Text("Hello");
		ImGui::End();
	}
}