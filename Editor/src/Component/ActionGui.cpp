#include "Component/ActionGui.h"
#include <imgui.h>
namespace GDEEditor
{
	void ActionGuiComponent::setup(const GDE::ComponentDescription& init_value)
	{
	}
	void ActionGuiComponent::update(const GDE::Timing& dt)
	{
		ImGuiIO& io = ImGui::GetIO();  // Store a reference
		ImVec2 screenSize = io.DisplaySize;
		ImGui::SetNextWindowPos(ImVec2(screenSize.x * 0.45, 0));
		ImGui::SetNextWindowSize(ImVec2(screenSize.x * 0.1, screenSize.y * 0.05 + 10));
		ImGui::Begin("Action", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
		
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0)); // No spacing between buttons

		if (ImGui::Button("Save", ImVec2(screenSize.x * 0.04, screenSize.y * 0.04)))
		{
		}		
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);  // 20px space
		if (ImGui::Button("Play", ImVec2(screenSize.x * 0.04, screenSize.y * 0.04)))
		{
		}
		    ImGui::PopStyleVar(); // Restore style var for other buttons

		ImGui::End();
	}
}