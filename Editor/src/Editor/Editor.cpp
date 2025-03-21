#include "Editor/Editor.h"
#include <source_dir.h>

#include <System/GuiSystem.h>
#include <System/DisplaySystem.h>
#include <System/InputSystem.h>
#include <System/LogicSystem.h>
#include <System/PhysicsSystem.h>
#include <System/AlternateLogicSystem.h>

#include "Editor/Component/ProjectTreeGuiComponent.h"
#include "Editor/Component/ProjectTemplateGuiComponent.h"
#include "Editor/Component/ActionGui.h"
#include "Editor/Component/PropertiesEditorGui.h"
#include "Editor/Component/SceneCleanerComponent.h"

#include "Common/ComponentRegister.h"

#include "Editor/System/EditorDisplaySystemImpl.h"
namespace GDEEditor
{
	Editor::Editor(const Arguments& arguments) : Game(
		arguments,
		"TP3 Editor",
		{640, 360},
		GDE::Maximized
	)
	{
		_imguiContext = ImGuiIntegration::Context(Vector2{ windowSize() } / dpiScaling(),
			windowSize(), framebufferSize());

		GL::Renderer::setBlendEquation(GL::Renderer::BlendEquation::Add,
			GL::Renderer::BlendEquation::Add);
		GL::Renderer::setBlendFunction(GL::Renderer::BlendFunction::SourceAlpha,
			GL::Renderer::BlendFunction::OneMinusSourceAlpha);
	}

	void Editor::setupSystem()
	{
		addSystem<GDE::AlternateLogicSystem>();
		addSystem<GDE::LogicSystem>();
		addSystem<GDE::GuiSystem>();
		addSystem<GDE::PhysicsSystem>();
		addSystem<GDE::DisplaySystem>();
		addSystem<GDE::InputSystem>();

		GDE::LogicSystem::getInstance().setEnable(false);
		GDE::PhysicsSystem::getInstance().setEnable(false);

		GDE::DisplaySystem::getInstance().initImpl([] { return std::make_unique<EditorDisplaySystemImpl>(); });
	}

	void Editor::registerComponent() const
	{
		GDE::Component::Register<ProjectTreeGuiComponent>();
		GDE::Component::Register<ProjectTemplateGuiComponent>();
		GDE::Component::Register<PropertiesEditorGuiComponent>();
		GDE::Component::Register<ActionGuiComponent>();
		GDE::Component::Register<SceneCleanerComponent>();

		registerCommonComponent();
	}

	void Editor::setupScene()
	{
		GDE::Scene::load(GDE::Descr::load(std::string(SOURCE_DIR) + "/Editor/data/Editor/scene/editor_default.yaml"));	
	}
}