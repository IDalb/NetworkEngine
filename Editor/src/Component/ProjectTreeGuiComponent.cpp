#include "Component/ProjectTreeGuiComponent.h"
#include <imgui.h>
#include <Game.h>
namespace GDEEditor
{

	void ProjectTreeGuiComponent::setup(const GDE::ComponentDescription& init_value)
	{
	}

	void ProjectTreeGuiComponent::update(const GDE::Timing& dt)
	{
		ImGuiIO& io = ImGui::GetIO();  // Store a reference
		ImVec2 screenSize = io.DisplaySize;
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(screenSize.x * 0.15, screenSize.y));
		ImGui::Begin("Project tree", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
		ImGui::Text("Hello");
		ImGui::End();
	}
}
