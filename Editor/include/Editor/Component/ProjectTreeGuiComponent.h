#pragma once
#include <Component/GuiComponent.h>
#include <Component/DisplayComponent.h>
#include <TypeDef.h>
namespace GDEEditor
{
	class ProjectTreeGuiComponent:
		public GDE::GuiComponent
	{
	public:
		static constexpr auto type = "ProjectTreeGui";

		ProjectTreeGuiComponent(GDE::Entity& owner) : Component(owner) {}
		~ProjectTreeGuiComponent() = default;
		ProjectTreeGuiComponent(const ProjectTreeGuiComponent&) = default;
		ProjectTreeGuiComponent& operator=(const ProjectTreeGuiComponent&) = default;
		ProjectTreeGuiComponent(ProjectTreeGuiComponent&&) = default;
		ProjectTreeGuiComponent& operator=(ProjectTreeGuiComponent&&) = default;


		void resetScene();
		void resetCamera();

		void setup(const GDE::ComponentDescription& init_value) override;

		void update(const GDE::Timing& dt) override;
		void save();

		void createObject(const std::string& templatePate, const Magnum::Vector3& position);
		
		// change to set position, ...
		void removeEntity(GDE::Entity* entity);
		void changeEntityValue(GDE::Entity* entity, GDE::Component* component, const std::string& component_name, const std::string& field_name, float value, int index = -1, std::string index_suffix = "");
	private:
		void recursiveConstructTree(GDE::EntityRef& entity);
		void createScene();

		GDE::Description _gameWorld;
		GDE::Object3D _cameraDefaultTransform;
	};
}