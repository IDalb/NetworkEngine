#include "Editor/Component/PropertiesEditorGui.h"
#include <imgui.h>
#include <Game.h>
namespace GDEEditor
{
	void PropertiesEditorGuiComponent::setup(const GDE::ComponentDescription& init_value)
	{
	}

	void PropertiesEditorGuiComponent::update(const GDE::Timing& dt)
	{
		ImGuiIO& io = ImGui::GetIO();  // Store a reference
		ImVec2 screenSize = io.DisplaySize;
		ImGui::SetNextWindowPos(ImVec2(screenSize.x * 0.85, 0));
		ImGui::SetNextWindowSize(ImVec2(screenSize.x * 0.15, screenSize.y));
		ImGui::Begin("Properties Editor", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
		ImGui::Text("Hello");
		ImGui::End();
	}
}
