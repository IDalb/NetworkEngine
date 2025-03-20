#include "Editor.h"
#include <source_dir.h>

#include <System/GuiSystem.h>
#include <System/DisplaySystem.h>
#include <System/InputSystem.h>

#include "Component/ProjectTreeGuiComponent.h"
#include "Component/ProjectTemplateGuiComponent.h"
#include "Component/ActionGui.h"
#include "Component/PropertiesEditorGui.h"
namespace GDEEditor
{
	Editor::Editor(const Arguments& arguments) : Game(
		arguments,
#ifndef SHIPPING
		"TP3 Editor",
		{640, 360},
		GDE::Maximized
#else
		"TP3 Game",
		{700, 700},
		GDE::Windowed
#endif
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
#ifndef SHIPPING
		addSystem<GDE::GuiSystem>();
		addSystem<GDE::DisplaySystem>();
		addSystem<GDE::InputSystem>();

#endif
	}

	void Editor::registerComponent() const
	{
#ifndef SHIPPING
		GDE::Component::Register<ProjectTreeGuiComponent>();
		GDE::Component::Register<ProjectTemplateGuiComponent>();
		GDE::Component::Register<PropertiesEditorGuiComponent>();
		GDE::Component::Register<ActionGuiComponent>();
#endif
	}

	void Editor::setupScene()
	{

#ifndef SHIPPING
		GDE::Scene::load(GDE::Descr::load(std::string(SOURCE_DIR) + "/Editor/data/Editor/scene/editor_default.yaml"));
#else
		GDE::Scene::load(GDE::Descr::load(std::string(SOURCE_DIR) + "/Editor/data/scene/Game/default.yaml"));
#endif // !SHIPPING
		
	}

	void Editor::viewportEvent(ViewportEvent& event)
	{
		GL::defaultFramebuffer.setViewport({ Vector2i(event.framebufferSize().x() * 0.15, 0), Vector2i(event.framebufferSize().x() * 0.7, event.framebufferSize().y() * 0.8) });
		GDE::GuiSystem::getInstance().relayout(Vector2{ event.windowSize() } / dpiScaling(), event.windowSize(), framebufferSize());

	}
}