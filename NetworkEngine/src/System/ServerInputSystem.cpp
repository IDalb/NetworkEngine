#include "System/ServerInputSystem.h"
namespace GDE
{
	ServerInputSystem& ServerInputSystem::getInstance()
	{
		static ServerInputSystem input_system;
		return input_system;
	}

	bool ServerInputSystem::isKeyPressed(Key::Key key, uint32_t id)
	{
		return keys_state_[id][key].state == State::PRESSED;
	}

	bool ServerInputSystem::isMouseButtonPressed(Mouse::Button button, uint32_t id)
	{
		return mouse_buttons_state_[id][button].state == State::PRESSED;
	}

	bool ServerInputSystem::isKeyReleased(Key::Key key, uint32_t id)
	{
		return keys_state_[id][key].state == State::RELEASED;
	}

	bool ServerInputSystem::isMouseButtonReleased(Mouse::Button button, uint32_t id)
	{
		return mouse_buttons_state_[id][button].state == State::RELEASED;
	}

	bool ServerInputSystem::isKeyHeld(Key::Key key, uint32_t id)
	{
		return keys_state_[id][key].state == State::HELD || keys_state_[id][key].state == State::PRESSED;
	}

	bool ServerInputSystem::isMouseButtonHeld(Mouse::Button button, uint32_t id)
	{
		return mouse_buttons_state_[id][button].state == State::HELD || mouse_buttons_state_[id][button].state == State::PRESSED;
	}

	int ServerInputSystem::getAxis(const Key::Key negative, const Key::Key positive, uint32_t id) {
		return (isKeyHeld(negative, id) ? -1 : 0) + (isKeyHeld(positive, id) ? 1 : 0);
	}


	Magnum::Vector2i ServerInputSystem::getScreenMousePosition(uint32_t id)
	{
		return _mousePos[id];
	}


	Magnum::Vector2 ServerInputSystem::getNormalizedMousePosition(uint32_t id)
	{
		return static_cast<Magnum::Vector2>(_mousePos[id]) / std::sqrt(_mousePos[id].dot());
	}

	Magnum::Vector2 ServerInputSystem::getMouseVelocity(uint32_t id)
	{
		return _mouseVelocity[id];
	}

	Magnum::Vector2 ServerInputSystem::getNormalizedMouseVelocity(uint32_t id)
	{
		return _mouseVelocity[id] / std::sqrt(_mouseVelocity[id].dot());
	}

	//int ServerInputSystem::getMouseScrollDirection(uint32_t id)
	//{
	//	if (mouse_scroll_value_ == 0)
	//	{
	//		return 0;
	//	}
	//	return mouse_scroll_value_ / std::abs(mouse_scroll_value_);
	//}
	//
	//float ServerInputSystem::getMouseScrollValue()
	//{
	//	return mouse_scroll_value_;
	//}
}