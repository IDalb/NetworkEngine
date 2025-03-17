#include "Editor.h"
#include <source_dir.h>


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
}