#include "Editor/Component/PropertiesEditorGui.h"
#include <imgui.h>
#include <Game.h>
#include <System/InputSystem.h>
#include <Component/CameraComponent.h>
#include <Component/TransformComponent.h>

#include "Functions/RayCast.h"
#include "System/PhysicsSystem.h"
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
	void PropertiesEditorGuiComponent::updateAlternateLogic(const GDE::Timing& timing)
	{
		auto& inputSystem = GDE::InputSystem::getInstance();
		if (inputSystem.isMouseButtonPressed(GDE::Mouse::LEFT))
		{
			auto camera = GDE::Scene::findEntityWithTag("camera")->getComponent<GDE::CameraComponent>();
			auto camera_transform = GDE::Scene::findEntityWithTag("camera")->getComponent<GDE::TransformComponent>();

			Vector2i framebufferSize = GDE::Game::_app->framebufferSize();

			// Define the viewport properties
			Vector2i viewportPos = Vector2i(framebufferSize.x() * 0.15f, 0);
			Vector2i viewportSize = Vector2i(framebufferSize.x() * 0.7, framebufferSize.y() * 0.7);

			// Get the mouse position
			Magnum::Vector2 mousePos = static_cast<Magnum::Vector2>(inputSystem.getScreenMousePosition());

			// Map the mouse position to the viewport space
			Magnum::Vector2 viewportMousePos = mousePos - Magnum::Vector2{viewportPos};

			// Normalize the mouse position relative to the viewport size
			Magnum::Vector2 normalizedPos = viewportMousePos / Magnum::Vector2{viewportSize};

			// Ensure it's within the valid range (0 to 1)
			if (normalizedPos.x() < 0.0f || normalizedPos.x() > 1.0f || normalizedPos.y() < 0.0f || normalizedPos.y() > 1.0f)
			{
				return; // Ignore raycast if outside of the viewport
			}

			// Convert normalized position to NDC space (-1 to 1)
			Magnum::Vector2 clickPoint = Vector2::yScale(-1.0f) * (normalizedPos - Magnum::Vector2{0.5f})* camera->getCamera()->projectionSize();

			// Calculate the ray direction
			Magnum::Vector3 direction = (camera_transform->getTransform().absoluteTransformation().rotationScaling() *
				Magnum::Vector3{clickPoint, -1.0f}).normalized();

			// Get the starting point (camera position)
			Magnum::Vector3 startPoint = camera_transform->getTransform().transformation().translation();

			// Perform the raycast
			GDE::RaycastResult result = GDE::performRaycast(GDE::PhysicsSystem::getInstance().getWorld(), startPoint, startPoint + direction * 100);

			if (result.body != nullptr)
			{
				printf(result.body->owner().getName().c_str());
				printf("\n");
			}


		}
	}
}
