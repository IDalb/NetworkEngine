#include "Editor/Component/SceneCleanerComponent.h"

#include <System/GuiSystem.h>
#include <System/DisplaySystem.h>

GDEEditor::SceneCleanerComponent::~SceneCleanerComponent()
{
	GDE::DisplaySystem::getInstance().setEnable(true);
	GDE::GuiSystem::getInstance().setEnable(true);
}
