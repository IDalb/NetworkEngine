#include "Editor/Component/ProjectTemplateGuiComponent.h"
#include <Game.h>
#include <imgui.h>
#include <System/InputSystem.h>

#include <Entity.h>
#include "Editor/Component/ProjectTreeGuiComponent.h"
namespace GDEEditor
{
	void ProjectTemplateGuiComponent::setup(const GDE::ComponentDescription& init_value)
	{

	}

	void ProjectTemplateGuiComponent::update(const GDE::Timing& dt)
	{
		ImGuiIO& io = ImGui::GetIO();  // Store a reference
		ImVec2 screenSize = io.DisplaySize;
		ImGui::SetNextWindowPos(ImVec2(screenSize.x * 0.15, screenSize.y * 0.7));
		ImGui::SetNextWindowSize(ImVec2(screenSize.x * 0.7, screenSize.y * 0.3));
		ImGui::Begin("Template drawer", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
		if (ImGui::Button("Cube", ImVec2(screenSize.y * 0.2, screenSize.y * 0.2)))
		{
			owner().getComponent<ProjectTreeGuiComponent>()->createObject(std::string(SOURCE_DIR) + "/Editor/data/Editor/template/Cube.yaml", Magnum::Vector3());
		}
		ImGui::SameLine();
		if (ImGui::Button("Sphere", ImVec2(screenSize.y * 0.2, screenSize.y * 0.2)))
		{
			owner().getComponent<ProjectTreeGuiComponent>()->createObject(std::string(SOURCE_DIR) + "/Editor/data/Editor/template/Sphere.yaml", Magnum::Vector3());
		}
		//if (ImGui::IsItemActive())
		//{
		//	_placeCube = true;
		//}
		ImGui::End();
	}
	void ProjectTemplateGuiComponent::updateAlternateLogic(const GDE::Timing& timing)
	{	
		//if (_placeCube && GDE::InputSystem::getInstance().isMouseButtonReleased(GDE::Mouse::LEFT))
		//{
		//}
		//if (_placeCube && GDE::InputSystem::getInstance().isMouseButtonHeld(GDE::Mouse::LEFT) == false)
		//{
		//	//_placeCube = false;
		//}
	}
}