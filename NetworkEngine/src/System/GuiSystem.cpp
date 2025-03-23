#include "System/GuiSystem.h"
#include "Component/GuiComponent.h"

#include "Entity.h"
#include "GlobalConstants.h"
#include "Game.h"
namespace GDE
{
	GuiSystem& GuiSystem::getInstance()
	{
		static GuiSystem display_system;
		return display_system;
	}

	void GuiSystem::setup(const Magnum::Vector2& size, const Magnum::Vector2i& windowSize, const Magnum::Vector2i& framebufferSize, Magnum::Platform::GlfwApplication* app)
	{
		_imguiContext = Magnum::ImGuiIntegration::Context(size,	windowSize, framebufferSize);
		_app = app;
	}

	void GuiSystem::relayout(const Magnum::Vector2& size, const Magnum::Vector2i& windowSize, const Magnum::Vector2i& framebufferSize)
	{
		_imguiContext.relayout(size, windowSize, framebufferSize);
	}

	void GuiSystem::remove()
	{
		_imguiContext = Magnum::ImGuiIntegration::Context({ Magnum::NoCreate });
	}

	bool GuiSystem::keyPressEvent(Magnum::Platform::GlfwApplication::KeyEvent& event)
	{
		if (_imguiContext.handleKeyPressEvent(event)) return true;
		return false;
	}

	bool GuiSystem::keyReleaseEvent(Magnum::Platform::GlfwApplication::KeyEvent& event)
	{
		if (_imguiContext.handleKeyReleaseEvent(event)) return true;
		return false;
	}

	bool GuiSystem::mousePressEvent(Magnum::Platform::GlfwApplication::MouseEvent& event)
	{
		if (_imguiContext.handleMousePressEvent(event)) return true;
		return false;
	}

	bool GuiSystem::mouseReleaseEvent(Magnum::Platform::GlfwApplication::MouseEvent& event)
	{
		if (_imguiContext.handleMouseReleaseEvent(event)) return true;
		return false;
	}

	bool GuiSystem::mouseMoveEvent(Magnum::Platform::GlfwApplication::MouseMoveEvent& event)
	{
		if (_imguiContext.handleMouseMoveEvent(event)) return true;
		return false;
	}

	bool GuiSystem::mouseScrollEvent(Magnum::Platform::GlfwApplication::MouseScrollEvent& event)
	{
		if (_imguiContext.handleMouseScrollEvent(event)) return true;
		return false;
	}

	bool GuiSystem::textInputEvent(Magnum::Platform::GlfwApplication::TextInputEvent& event)
	{
		if (_imguiContext.handleTextInputEvent(event)) return true;
		return false;
	}


	void GuiSystem::iterate(const Timing& dt)
	{
		_imguiContext.newFrame();
		if (ImGui::GetIO().WantTextInput && !GDE::Game::_app->isTextInputActive())
			GDE::Game::_app->startTextInput();
		else if (!ImGui::GetIO().WantTextInput && GDE::Game::_app->isTextInputActive())
			GDE::Game::_app->stopTextInput();

		for (auto& display_component : _guiComponents)
		{
			if (display_component->enabled() && display_component->owner().active())
			{
				display_component->update(dt);
			}
		}
		_imguiContext.updateApplicationCursor(*_app);


	}

	void GuiSystem::registerComponent(GuiComponent* display_component)
	{
		_guiComponents.insert(display_component);
	}

	void GuiSystem::removeComponent(GuiComponent* display_component)
	{
		_guiComponents.erase(display_component);
	}
}