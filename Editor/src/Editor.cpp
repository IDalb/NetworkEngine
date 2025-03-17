#include "Editor.h"
#include <source_dir.h>


#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/GL/Version.h>
#include <Magnum/Math/Color.h>
namespace GDEEditor
{
	Editor::Editor(const Arguments& arguments) : Game(
		arguments,
#ifndef SHIPPING
		"TP3 Editor",
#else
		"TP3 Game",
#endif
		{ 640, 360})
	{
	}

	void Editor::setupSystem()
	{
#ifndef SHIPPING

#endif
	}

	void Editor::registerComponent() const
	{
#ifndef SHIPPING

#endif
	}

	void Editor::setupScene()
	{

#ifndef SHIPPING
		GDE::Scene::load(GDE::Descr::load(std::string(SOURCE_DIR) + "/Editor/data/scene/Editor/editor_default.yaml"));
#else
		GDE::Scene::load(GDE::Descr::load(std::string(SOURCE_DIR) + "/Editor/data/scene/Game/default.yaml"));
#endif // !SHIPPING

	}

	void Editor::drawEvent()
	{
		GL::defaultFramebuffer.clear(GL::FramebufferClear::Color | GL::FramebufferClear::Depth);


		swapBuffers();
	}
}