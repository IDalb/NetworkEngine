#include "Editor/Component/PropertiesEditorGui.h"
#include "Editor/Component/ProjectTreeGuiComponent.h"
#include <imgui.h>
#include <Game.h>
#include <System/InputSystem.h>
#include <Component/CameraComponent.h>
#include <Component/TransformComponent.h>

#include "Functions/RayCast.h"
#include "System/PhysicsSystem.h"

#include <Entity.h>

#include <Magnum/GlmIntegration/Integration.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
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
		if (_selectedEntity)
		{
			ImGui::Text(_selectedEntity->getName().c_str());
			auto transformComponent = _selectedEntity->getComponent<GDE::TransformComponent>();
			if (ImGui::TreeNodeEx(transformComponent->type, ImGuiTreeNodeFlags_DefaultOpen))
			{
				constexpr std::array<const char*, 3> POS = { "x", "y", "z" };
				for (auto& property : _transformValue)
				{
					if (ImGui::TreeNodeEx(property.first.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
					{
						int index = 0;
						for (auto& pair : property.second)
						{
							ImGui::AlignTextToFramePadding();  // Align text to the left
							ImGui::Text(pair.first.c_str());          // Label
							ImGui::SameLine();                 // Keep next widget on the same line
							float min = 0;
							float max = 0;
							float step = 0.5;
							if (property.first == "scale")
							{
								step = 0.05;
								min = step;
								max = FLT_MAX;
							}

							if (ImGui::DragFloat(("##" + property.first + pair.first).c_str(), &pair.second, step, min, max, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
							{
								owner().getComponent<ProjectTreeGuiComponent>()->changeEntityValue(_selectedEntity, transformComponent, transformComponent->type, property.first, pair.second, index, POS[index]);
							}
							index++;
						}
						ImGui::TreePop();
					}
				}
				ImGui::TreePop();
			}
		}
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
				selectEntity(&result.body->owner());
			}
			else
			{
				removeSelection();
			}
		}
	}
	void PropertiesEditorGuiComponent::selectEntity(GDE::Entity* entity)
	{
		_selectedEntity = entity;
		auto& transform = _selectedEntity->getComponent<GDE::TransformComponent>()->getTransform();

		// Example transformation matrix (this should come from your object)
		glm::mat4 transformation = glm::mat4(transform.transformationMatrix()); // Assuming object.transformationMatrix() returns a glm::mat4

		// Decompose the transformation matrix into translation, rotation, and scale
		glm::vec3 translation, scale;
		glm::quat rotation;
		glm::mat3 rotationMatrix = glm::mat3(transformation);
		glm::vec3 skew;
		glm::vec4 perspective;
		// Decompose the matrix
		glm::decompose(transformation, scale, rotation, translation, skew, perspective);

		// Convert the quaternion to Euler angles if needed
		glm::vec3 eulerAngles = glm::eulerAngles(rotation);
		eulerAngles = glm::degrees(eulerAngles);  // Convert to degrees if desired

		_transformValue["position"]["x"] = translation.x;
		_transformValue["position"]["y"] = translation.y;
		_transformValue["position"]["z"] = translation.z;

		_transformValue["rotation"]["x"] = eulerAngles.x;
		_transformValue["rotation"]["y"] = eulerAngles.y;
		_transformValue["rotation"]["z"] = eulerAngles.z;

		_transformValue["scale"]["x"] = scale.x;
		_transformValue["scale"]["y"] = scale.y;
		_transformValue["scale"]["z"] = scale.z;
	}
}
