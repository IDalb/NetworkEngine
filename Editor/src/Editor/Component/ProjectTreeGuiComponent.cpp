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
	void recursiveConstructTree(GDE::EntityRef& entity)
	{
		if (entity->getComponent<GDE::TransformComponent>())
		{
			if (entity->getChildren().size() == 0)
			{
				ImGui::Text(("   " + entity->getName()).c_str());
#ifdef DEBUG
				if (ImGui::IsItemClicked())
				{
					printf(entity->getName().c_str());
					printf("\n");
				}
#endif
			}
			else
			{
				if (ImGui::TreeNodeEx(entity->getName().c_str(), ImGuiTreeNodeFlags_DefaultOpen))
				{
#ifdef DEBUG
					if (ImGui::IsItemClicked())
					{
						printf(entity->getName().c_str());
						printf("\n");
					}
#endif
					for (auto& child : entity->getChildren())
					{
						recursiveConstructTree(child);
					}
					ImGui::TreePop();
				}
			}
		}
	}
	void ProjectTreeGuiComponent::resetScene()
	{
		owner().removeChild(owner().getChildren()[0]);
		createScene();
	}
	void ProjectTreeGuiComponent::resetCamera()
	{
		GDE::Scene::findEntityWithTag("camera")->getComponent<GDE::TransformComponent>()->getTransform().setTransformation(_cameraDefaultTransform.transformation());
	}
	void ProjectTreeGuiComponent::setup(const GDE::ComponentDescription& init_value)
	{
		_gameWorld = GDE::Descr::load(std::string(SOURCE_DIR) + "/Editor/data/Game/scene/scene.yaml");

		createScene();
		_cameraDefaultTransform.setTransformation(GDE::Scene::findEntityWithTag("camera")->getComponent<GDE::TransformComponent>()->getTransform().transformation());

	}

	void ProjectTreeGuiComponent::update(const GDE::Timing& dt)
	{
		ImGuiIO& io = ImGui::GetIO();  // Store a reference
		ImVec2 screenSize = io.DisplaySize;
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(screenSize.x * 0.15, screenSize.y));
		ImGui::Begin("Project tree", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
		for (auto& child : owner().getChildren()[0]->getChildren())
		{
			recursiveConstructTree(child);
		}
		ImGui::End();
	}
	void ProjectTreeGuiComponent::save()
	{
		std::ofstream fout(std::string(SOURCE_DIR) + "/Editor/data/Game/scene/scene.yaml");
		fout << _gameWorld;
		fout.close();
	}
	void ProjectTreeGuiComponent::createObject(const std::string& templatePate, const Magnum::Vector3& position)
	{
		GDE::Description descr = GDE::Descr::load(templatePate);
		descr[0]["description"]["components"]["Transform"]["position"][0] = position.x();
		descr[0]["description"]["components"]["Transform"]["position"][1] = position.y();
		descr[0]["description"]["components"]["Transform"]["position"][2] = position.z();
		auto newObject = GDE::Scene::createEntity(descr[0], owner().getChildren()[0]);
		_gameWorld.push_back(descr[0]);
	}

	void ProjectTreeGuiComponent::createScene()
	{
		auto child = GDE::Scene::createEntity(GDE::Descr::load(std::string(SOURCE_DIR) + "/Editor/data/Editor/template/SceneCleaner.yaml")[0], owner().shared_from_this());
		for (const auto& node : _gameWorld)
		{
			GDE::Scene::createEntity(node, child);
		}

	}
}
