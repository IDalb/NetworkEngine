#pragma once
#include <System/System.h>

#include <std.h>
#include <Magnum/Math/Vector2.h>
#include <KeyCode.h>
namespace GDE
{
	class ServerInputSystem
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

		struct InputKeyData
		{
			uint32_t playerID;
			Key::Key key;
			bool pressed;
		};
		struct InputMouseButtonData
		{
			uint32_t playerID;
			Mouse::Button Button;
			bool pressed;
		};
		struct InputMousePositionData
		{
			uint32_t playerID;
			Magnum::Vector2i position;
		};
		struct InputMouseVelocityData
		{
			uint32_t playerID;
			Magnum::Vector2 velocity;
		};

		inline static float dead_zone_ = 0.1;

		ServerInputSystem() = default;
		ServerInputSystem(const ServerInputSystem&) = default;
		ServerInputSystem(ServerInputSystem&&) = default;
		ServerInputSystem& operator=(const ServerInputSystem&) = default;
		ServerInputSystem& operator=(ServerInputSystem&&) = default;

		~ServerInputSystem() = default;

		static ServerInputSystem& getInstance();

		void iterate(const Timing& dt) override
		{
			std::sort(_inputKey.begin(), _inputKey.end(), [](const auto& a, const auto& b) {
				return a.first < b.first;
				});
			std::sort(_inputMouseButton.begin(), _inputMouseButton.end(), [](const auto& a, const auto& b) {
				return a.first < b.first;
				});
			std::sort(_inputMousePos.begin(), _inputMousePos.end(), [](const auto& a, const auto& b) {
				return a.first < b.first;
				});
			std::sort(_inputMouseVelocity.begin(), _inputMouseVelocity.end(), [](const auto& a, const auto& b) {
				return a.first < b.first;
				});

			for (auto& pair : _inputKey)
			{
				int i = -1;
				if (pair.first >= dt._frame)
				{
					i++;
					setKeyState(pair.second.key, State::Value(pair.second.pressed), pair.second.playerID);
				}
				if(i > -1)
				{
					_inputKey.erase(_inputKey.begin(), _inputKey.begin() + i);
				}
			}

			for (auto& pair : _inputMouseButton)
			{
				int i = -1;
				if (pair.first >= dt._frame)
				{
					i++;
					setMouseButtonState(pair.second.Button, State::Value(pair.second.pressed), pair.second.playerID);
				}
				if (i > -1)
				{
					_inputMouseButton.erase(_inputMouseButton.begin(), _inputMouseButton.begin() + i);
				}
			}

			for (auto& pair : _inputMousePos)
			{
				int i = -1;
				if (pair.first >= dt._frame)
				{
					i++;
					setMousePos(pair.second.position, pair.second.playerID);
				}
				if (i > -1)
				{
					_inputMousePos.erase(_inputMousePos.begin(), _inputMousePos.begin() + i);
				}
			}

			for (auto& pair : _inputMouseVelocity)
			{
				int i = -1;
				if (pair.first >= dt._frame)
				{
					i++;
					setMouseVelocity(pair.second.velocity, pair.second.playerID);
				}
				if (i > -1)
				{
					_inputMouseVelocity.erase(_inputMouseVelocity.begin(), _inputMouseVelocity.begin() + i);
				}
			}

			
			for(auto& pair : updated_keys_)
			{
				for (auto& key : pair.second)
				{
					switch (keys_state_.at(pair.first).at(key).state)
					{
					case State::PRESSED:
						keys_state_.at(pair.first).at(key).state = State::HELD;
						break;
					case State::RELEASED:
						keys_state_.at(pair.first).at(key).state = State::NONE;
						break;
					}
				}
			}
			updated_keys_.clear();

			for (auto& pair : updated_mouse_buttons_)
			{
				for (auto& button : pair.second)
				{
					switch (mouse_buttons_state_.at(pair.first).at(button).state)
					{
					case State::PRESSED:
						mouse_buttons_state_.at(pair.first).at(button).state = State::HELD;
						break;
					case State::RELEASED:
						mouse_buttons_state_.at(pair.first).at(button).state = State::NONE;
						break;
					}
				}
				updated_mouse_buttons_[pair.first].clear();
			}
			
			//mouse_scroll_value_ = 0;
			//_mouseVelocity = {0, 0};
		}

		bool isKeyPressed(Key::Key key, uint32_t id);
		bool isMouseButtonPressed(Mouse::Button button, uint32_t id);
		
		bool isKeyReleased(Key::Key key, uint32_t id);
		bool isMouseButtonReleased(Mouse::Button button, uint32_t id);

		bool isKeyHeld(Key::Key key, uint32_t id);
		bool isMouseButtonHeld(Mouse::Button button, uint32_t id);

		// In main window coords
		Magnum::Vector2i getScreenMousePosition(uint32_t id);
		Magnum::Vector2 getNormalizedMousePosition(uint32_t id);

		Magnum::Vector2 getMouseVelocity(uint32_t id);
		Magnum::Vector2 getNormalizedMouseVelocity(uint32_t id);

		// -1 = 'negative' key; 1 = 'positive' key; 0 = both
		//int getAxis(Key::Key negative, Key::Key positive);

		//// -1 scroll down, 1 scroll up
		//int getMouseScrollDirection();
		//// < 0 scroll down, > 0 scroll up
		//float getMouseScrollValue();

		void setKeyState(Key::Key key, bool value, uint32_t id, uint32_t frame)
		{
			_inputKey.push_back({ frame, { id, key, value } });
		}

		void setKeyState(Key::Key key, bool value, uint32_t id)
		{ 
			keys_state_[id][key].state = State::Value(value); 
			updated_keys_[id].push_back(key);
		}

		void setMouseButtonState(Mouse::Button button, bool value, uint32_t id, uint32_t frame)
		{
			_inputMouseButton.push_back({ frame, { id, button, value } });
		}
		void setMouseButtonState(Mouse::Button button, bool value, uint32_t id)
		{
			mouse_buttons_state_[id][button] = State::Value(value);
			updated_mouse_buttons_[id].push_back(button);
		}

		void setMouseScrollValue(float value)
		{
			//mouse_scroll_value_ = value;
		}

		void setMousePos(const Magnum::Vector2i& pos, uint32_t id, uint32_t frame)
		{
			_inputMousePos.push_back({ frame, {id, pos} });
		}

		void setMouseVelocity(const Magnum::Vector2& vel, uint32_t id, uint32_t frame)
		{
			_inputMouseVelocity.push_back({ frame, {id, vel} });
		}
		
		void setMousePos(const Magnum::Vector2i& pos, uint32_t id)
		{
			_mousePos[id] = pos;
		}

		void setMouseVelocity(const Magnum::Vector2& vel, uint32_t id)
		{
			_mouseVelocity[id] = vel;
		}

	private:
		// key : frame
		std::vector<std::pair<uint32_t, InputKeyData>> _inputKey;
		std::vector<std::pair<uint32_t, InputMouseButtonData>> _inputMouseButton;
		std::vector<std::pair<uint32_t, InputMousePositionData>> _inputMousePos;
		std::vector<std::pair<uint32_t, InputMouseVelocityData>> _inputMouseVelocity;

		// key : player id
		std::map<uint32_t, std::unordered_map<Key::Key, State>> keys_state_{};
		std::map<uint32_t, std::unordered_map<Mouse::Button, State>> mouse_buttons_state_{};

		std::map<uint32_t, std::vector<Key::Key>> updated_keys_{};
		std::map<uint32_t, std::vector<Mouse::Button>> updated_mouse_buttons_{};

		//float mouse_scroll_value_{};
		std::map<uint32_t, Magnum::Vector2i> _mousePos;
		std::map<uint32_t, Magnum::Vector2> _mouseVelocity;
	};
}

