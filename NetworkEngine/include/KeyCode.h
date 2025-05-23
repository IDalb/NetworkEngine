#pragma once

namespace GDE
{
	// Base on glfw key code
	namespace Key
	{
		enum class Key : uint16_t
		{
			/* Printable keys */
			SPACE = 32,
			APOSTROPHE = 39,  /* ' */
			COMMA = 44,  /* , */
			MINUS = 45,  /* - */
			PERIOD = 46,  /* . */
			SLASH = 47,  /* / */
			NUM_0 = 48,
			NUM_1 = 49,
			NUM_2 = 50,
			NUM_3 = 51,
			NUM_4 = 52,
			NUM_5 = 53,
			NUM_6 = 54,
			NUM_7 = 55,
			NUM_8 = 56,
			NUM_9 = 57,
			SEMICOLON = 59,  /* ; */
			EQUAL = 61,  /* = */
			A = 65,
			B = 66,
			C = 67,
			D = 68,
			E = 69,
			F = 70,
			G = 71,
			H = 72,
			I = 73,
			J = 74,
			K = 75,
			L = 76,
			M = 77,
			N = 78,
			O = 79,
			P = 80,
			Q = 81,
			R = 82,
			S = 83,
			T = 84,
			U = 85,
			V = 86,
			W = 87,
			X = 88,
			Y = 89,
			Z = 90,
			LEFT_BRACKET = 91,  /* [ */
			BACKSLASH = 92,  /* \ */
			RIGHT_BRACKET = 93,  /* ] */
			GRAVE_ACCENT = 96,  /* ` */
			WORLD_1 = 161, /* non-US #1 */
			WORLD_2 = 162, /* non-US #2 */

			/* Function keys */
			ESCAPE = 256,
			ENTER = 257,
			TAB = 258,
			BACKSPACE = 259,
			INSERT = 260,
			DEL = 261,
			RIGHT = 262,
			LEFT = 263,
			DOWN = 264,
			UP = 265,
			PAGE_UP = 266,
			PAGE_DOWN = 267,
			HOME = 268,
			END = 269,
			CAPS_LOCK = 280,
			SCROLL_LOCK = 281,
			NUM_LOCK = 282,
			PRINT_SCREEN = 283,
			PAUSE = 284,
			F1 = 290,
			F2 = 291,
			F3 = 292,
			F4 = 293,
			F5 = 294,
			F6 = 295,
			F7 = 296,
			F8 = 297,
			F9 = 298,
			F10 = 299,
			F11 = 300,
			F12 = 301,
			F13 = 302,
			F14 = 303,
			F15 = 304,
			F16 = 305,
			F17 = 306,
			F18 = 307,
			F19 = 308,
			F20 = 309,
			F21 = 310,
			F22 = 311,
			F23 = 312,
			F24 = 313,
			F25 = 314,
			NUMPAD_0 = 320,
			NUMPAD_1 = 321,
			NUMPAD_2 = 322,
			NUMPAD_3 = 323,
			NUMPAD_4 = 324,
			NUMPAD_5 = 325,
			NUMPAD_6 = 326,
			NUMPAD_7 = 327,
			NUMPAD_8 = 328,
			NUMPAD_9 = 329,
			NUMPAD_DECIMAL = 330,
			NUMPAD_DIVIDE = 331,
			NUMPAD_MULTIPLY = 332,
			NUMPAD_SUBTRACT = 333,
			NUMPAD_ADD = 334,
			NUMPAD_ENTER = 335,
			NUMPAD_EQUAL = 336,
			LEFT_SHIFT = 340,
			LEFT_CONTROL = 341,
			LEFT_ALT = 342,
			LEFT_SUPER = 343,
			RIGHT_SHIFT = 344,
			RIGHT_CONTROL = 345,
			RIGHT_ALT = 346,
			RIGHT_SUPER = 347,
			MENU = 348,
		};
	}

	namespace Mouse
	{
		enum class Button : uint8_t
		{
			BUTTON_1 = 0,
			BUTTON_2 = 1,
			BUTTON_3 = 2,
			BUTTON_4 = 3,
			BUTTON_5 = 4,
			BUTTON_6 = 5,
			BUTTON_7 = 6,
			BUTTON_8 = 7,

			LEFT = BUTTON_1,
			RIGHT = BUTTON_2,
			MIDDLE = BUTTON_3
		};
	}

	namespace Joystick
	{
		enum Joystick
		{
			JOYSTICK_1 = 0,
			JOYSTICK_2 = 1,
			JOYSTICK_3 = 2,
			JOYSTICK_4 = 3,
			JOYSTICK_5 = 4,
			JOYSTICK_6 = 5,
			JOYSTICK_7 = 6,
			JOYSTICK_8 = 7,
			JOYSTICK_9 = 8,
			JOYSTICK_10 = 9,
			JOYSTICK_11 = 10,
			JOYSTICK_12 = 11,
			JOYSTICK_13 = 12,
			JOYSTICK_14 = 13,
			JOYSTICK_15 = 14,
			JOYSTICK_16 = 15
		};
	}

	namespace Gamepad
	{
		enum Gamepad
		{
			A = 0,
			B = 1,
			X = 2,
			Y = 3,
			LEFT_BUMPER = 4,
			RIGHT_BUMPER = 5,
			BACK = 6,
			START = 7,
			GUIDE = 8,
			LEFT_THUMB = 9,
			RIGHT_THUMB = 10,
			DPAD_UP = 11,
			DPAD_RIGHT = 12,
			DPAD_DOWN = 13,
			DPAD_LEFT = 14,
			
			CROSS = A,
			CIRCLE = B,
			SQUARE = X,
			TRIANGLE = Y
		};
	}

	namespace Axis
	{
		enum Axis
		{
			LEFT_X = 0,
			LEFT_Y = 1,
			RIGHT_X = 2,
			RIGHT_Y = 3,
			LEFT_TRIGGER = 4,
			RIGHT_TRIGGER = 5
		};
	}
}