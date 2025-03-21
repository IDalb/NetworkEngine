#include "Editor/Component/ProjectTreeGuiComponent.h"
#include <imgui.h>
#include <Game.h>
#include <Scene.h>
#include <Entity.h>
#include <Component/LogicComponent.h>
#include <Component/TransformComponent.h>
#include <Component/CameraComponent.h>
#include <System/DisplaySystem.h>
#include <System/GuiSystem.h>
#include <TypeDef.h>
namespace GDEEditor
{
	void ProjectTreeGuiComponent::resetScene()
	{
		owner().removeChild(owner().getChildren()[0]);
		auto child = GDE::Scene::createEntity(GDE::Descr::load(std::string(SOURCE_DIR) + "/Editor/data/Editor/template/SceneCleaner.yaml")[0], owner().shared_from_this());
		for (const auto& node : _gameWorld)
		{
			GDE::Scene::createEntity(node, child);
		}
		GDE::DisplaySystem::getInstance().setEnable(false);
		GDE::GuiSystem::getInstance().setEnable(false);
	}
	void ProjectTreeGuiComponent::resetCamera()
	{
		GDE::Scene::findEntityWithTag("camera")->getComponent<GDE::TransformComponent>()->getTransform().setTransformation(_cameraDefaultTransform.transformation());
	}
	void ProjectTreeGuiComponent::setup(const GDE::ComponentDescription& init_value)
	{
		_gameWorld = GDE::Descr::load(std::string(SOURCE_DIR) + "/Editor/data/Game/scene/scene.yaml");

		GDE::Scene::createEntity(GDE::Descr::load(std::string(SOURCE_DIR) + "/Editor/data/Editor/template/SceneCleaner.yaml")[0], owner().shared_from_this());
		for (const auto& node : _gameWorld)
		{
			GDE::Scene::createEntity(node, owner().getChild("SceneCleaner"));
		}

		_cameraDefaultTransform.setTransformation(GDE::Scene::findEntityWithTag("camera")->getComponent<GDE::TransformComponent>()->getTransform().transformation());
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
