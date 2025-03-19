#pragma once
#include "System.h"

#include "std.h"
#include <Magnum/Math/Vector2.h>
#include "KeyCode.h"
namespace GDE
{
	class InputSystem
		: public System
	{
	public:
		struct State
		{
			enum Value
			{
				RELEASED = 0, PRESSED = 1, HELD, NONE
			};
			Value state;

			State(Value value = NONE) : state(value){}
		};

		inline static float dead_zone_ = 0.1;

		InputSystem() = default;
		InputSystem(const InputSystem&) = default;
		InputSystem(InputSystem&&) = default;
		InputSystem& operator=(const InputSystem&) = default;
		InputSystem& operator=(InputSystem&&) = default;

		~InputSystem() = default;

		static InputSystem& getInstance();

		void iterate(const Timing& dt) override
		{
			for (auto& key : updated_keys_)
			{
				switch (keys_state_.at(key).state)
				{
				case State::PRESSED:
					keys_state_.at(key).state = State::HELD;
					break;
				case State::RELEASED:
					keys_state_.at(key).state = State::NONE;
					break;
				}
			}			
			updated_keys_.clear();

			for (auto& button : updated_mouse_buttons_)
			{
				switch (mouse_buttons_state_.at(button).state)
				{
				case State::PRESSED:
					mouse_buttons_state_.at(button).state = State::HELD;
					break;
				case State::RELEASED:
					mouse_buttons_state_.at(button).state = State::NONE;
					break;
				}
			}			
			updated_mouse_buttons_.clear();
			
			mouse_scroll_value_ = 0;
		}

		bool isKeyPressed(Key::Key key);
		bool isMouseButtonPressed(Mouse::Button button);
		
		bool isKeyReleased(Key::Key key);
		bool isMouseButtonReleased(Mouse::Button button);

		bool isKeyHeld(Key::Key key);
		bool isMouseButtonHeld(Mouse::Button button);

		// In main window coords
		Magnum::Vector2i getScreenMousePosition();
		Magnum::Vector2 getNormalizedMousePosition();

		// -1 scroll down, 1 scroll up
		int getMouseScrollDirection();
		// < 0 scroll down, > 0 scroll up
		float getMouseScrollValue();

		void setKeyState(Key::Key key, bool value)
		{ 
			keys_state_[key].state = State::Value(value); 
			updated_keys_.push_back(key);
		}

		void setMouseButtonState(Mouse::Button button, bool value)
		{
			mouse_buttons_state_[button] = State::Value(value);
			updated_mouse_buttons_.push_back(button);
		}

		void setMouseScrollValue(float value)
		{
			mouse_scroll_value_ = value;
		}

		void setMousePos(const Magnum::Vector2i& pos)
		{
			_mousePos = pos;
		}

	private:
		std::unordered_map<Key::Key, State> keys_state_{};
		std::unordered_map<Mouse::Button, State> mouse_buttons_state_{};

		std::vector<Key::Key> updated_keys_{};
		std::vector<Mouse::Button> updated_mouse_buttons_{};

		float mouse_scroll_value_{};
		Magnum::Vector2i _mousePos;
	};
}

