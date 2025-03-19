#include "System/InputSystem.h"
namespace GDE
{
	InputSystem& InputSystem::getInstance()
	{
		static InputSystem input_system;
		return input_system;
	}

	bool InputSystem::isKeyPressed(Key::Key key)
	{
		return keys_state_[key].state == State::PRESSED;
	}

	bool InputSystem::isMouseButtonPressed(Mouse::Button button)
	{
		return mouse_buttons_state_[button].state == State::PRESSED;
	}

	bool InputSystem::isKeyReleased(Key::Key key)
	{
		return keys_state_[key].state == State::RELEASED;
	}

	bool InputSystem::isMouseButtonReleased(Mouse::Button button)
	{
		return mouse_buttons_state_[button].state == State::RELEASED;
	}

	bool InputSystem::isKeyHeld(Key::Key key)
	{
		return keys_state_[key].state == State::HELD || keys_state_[key].state == State::PRESSED;
	}

	bool InputSystem::isMouseButtonHeld(Mouse::Button button)
	{
		return mouse_buttons_state_[button].state == State::HELD ||mouse_buttons_state_[button].state == State::PRESSED;
	}

	Magnum::Vector2i InputSystem::getScreenMousePosition()
	{
		return _mousePos;
	}

	Magnum::Vector2 InputSystem::getNormalizedMousePosition()
	{
		return static_cast<Magnum::Vector2>(_mousePos) / std::sqrt(_mousePos.dot());
	}

	int InputSystem::getMouseScrollDirection()
	{
		if (mouse_scroll_value_ == 0)
		{
			return 0;
		}
		return mouse_scroll_value_ / std::abs(mouse_scroll_value_);
	}

	float InputSystem::getMouseScrollValue()
	{
		return mouse_scroll_value_;
	}
}