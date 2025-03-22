#include "Editor/Component/ActionGui.h"
#include <imgui.h>
#include <Entity.h>
#include "Editor/Component/ProjectTreeGuiComponent.h"
#include "Editor/Component/ProjectTemplateGuiComponent.h"
#include "Editor/Component/PropertiesEditorGui.h"

#include <System/AlternateLogicSystem.h>
#include <System/LogicSystem.h>
#include <System/GuiSystem.h>
#include <System/PhysicsSystem.h>
#include <System/InputSystem.h>

#include <Magnum/BulletIntegration/Integration.h>
#include <Magnum/BulletIntegration/MotionState.h>
#include <btBulletDynamicsCommon.h>

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
		ImGui::SetNextWindowSize(ImVec2(screenSize.x * 0.1, screenSize.y * 0.06));
		ImGui::Begin("Action", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
		
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0)); // No spacing between buttons

		if (ImGui::Button("Save", ImVec2(screenSize.x * 0.04, screenSize.y * 0.04)))
		{
			owner().getComponent<ProjectTreeGuiComponent>()->save();
		}		
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);  // 20px space
		if (_playing)
		{
			if (ImGui::Button("Stop", ImVec2(screenSize.x * 0.04, screenSize.y * 0.04)))
			{
				_playing = false;
				GDE::PhysicsSystem::getInstance().setEnable(false);
				GDE::LogicSystem::getInstance().setEnable(false);
				GDE::AlternateLogicSystem::getInstance().setEnable(true);

				owner().getComponent<ProjectTreeGuiComponent>()->resetScene();
			}
		}
		else
		{
			if (ImGui::Button("Play", ImVec2(screenSize.x * 0.04, screenSize.y * 0.04)))
			{
				_playing = true;
				GDE::PhysicsSystem::getInstance().setEnable(true);
				GDE::LogicSystem::getInstance().setEnable(true);
				GDE::AlternateLogicSystem::getInstance().setEnable(false);

				owner().getComponent<ProjectTreeGuiComponent>()->resetCamera();
			}
		}
		
		ImGui::PopStyleVar(); // Restore style var for other buttons

		ImGui::End();
	}
}