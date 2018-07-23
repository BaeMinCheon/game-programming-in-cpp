// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "InputSystem.h"
#include <SDL/SDL.h>
#include <cstring>
#include <fstream>

bool KeyboardState::GetKeyValue(SDL_Scancode keyCode) const
{
	return mCurrState[keyCode] == 1;
}

ButtonState KeyboardState::GetKeyState(SDL_Scancode keyCode) const
{
	if (mPrevState[keyCode] == 0)
	{
		if (mCurrState[keyCode] == 0)
		{
			return ENone;
		}
		else
		{
			return EPressed;
		}
	}
	else // Prev state must be 1
	{
		if (mCurrState[keyCode] == 0)
		{
			return EReleased;
		}
		else
		{
			return EHeld;
		}
	}
}

bool MouseState::GetButtonValue(int button) const
{
	return (SDL_BUTTON(button) & mCurrButtons) == 1;
}

ButtonState MouseState::GetButtonState(int button) const
{
	int mask = SDL_BUTTON(button);
	if ((mask & mPrevButtons) == 0)
	{
		if ((mask & mCurrButtons) == 0)
		{
			return ENone;
		}
		else
		{
			return EPressed;
		}
	}
	else
	{
		if ((mask & mCurrButtons) == 0)
		{
			return EReleased;
		}
		else
		{
			return EHeld;
		}
	}
}

bool ControllerState::GetButtonValue(SDL_GameControllerButton button) const
{
	return mCurrButtons[button] == 1;
}

ButtonState ControllerState::GetButtonState(SDL_GameControllerButton button) const
{
	if (mPrevButtons[button] == 0)
	{
		if (mCurrButtons[button] == 0)
		{
			return ENone;
		}
		else
		{
			return EPressed;
		}
	}
	else // Prev state must be 1
	{
		if (mCurrButtons[button] == 0)
		{
			return EReleased;
		}
		else
		{
			return EHeld;
		}
	}
}

bool InputSystem::Initialize()
{
	// Keyboard
	// Assign current state pointer
	mState.Keyboard.mCurrState = SDL_GetKeyboardState(NULL);
	// Clear previous state memory
	memset(mState.Keyboard.mPrevState, 0,
		SDL_NUM_SCANCODES);

	// Mouse (just set everything to 0)
	mState.Mouse.mCurrButtons = 0;
	mState.Mouse.mPrevButtons = 0;

	for (int i = 0; i < 4; ++i)
	{
		if(SDL_IsGameController(i))
		{
			auto controller = SDL_GameControllerOpen(i);
			mControllers[i] = controller;
		}

		mState.Controllers[i].mIsConnected =
			(mControllers[i] != nullptr);
		memset(mState.Controllers[i].mCurrButtons,
			0, SDL_CONTROLLER_BUTTON_MAX);
		memset(mState.Controllers[i].mPrevButtons,
			0, SDL_CONTROLLER_BUTTON_MAX);
	}

	LoadMapping();

	return true;
}

void InputSystem::LoadMapping()
{
	std::fstream fs;
	fs.open("Assets/KeyMapping.txt");
	if (!fs.is_open())
	{
		SDL_Log("Failed to load key mapping.");
	}

	char buffer[256];
	while (fs.getline(buffer, 256))
	{
		std::string mapping(buffer);
		std::size_t index = -1;

		index = mapping.find(",");
		std::string actionName = mapping.substr(0, index);
		mapping.erase(0, index + 1);

		index = mapping.find(",");
		std::string deviceName = mapping.substr(0, index);
		mapping.erase(0, index + 1);

		int value = stoi(mapping);
		SDL_Scancode code = static_cast<SDL_Scancode>(value);

		mState.KBMap[actionName] = code;
	}

	fs.close();
}

void InputSystem::Shutdown()
{
}

void InputSystem::PrepareForUpdate()
{
	// Copy current state to previous
	// Keyboard
	memcpy(mState.Keyboard.mPrevState,
		mState.Keyboard.mCurrState,
		SDL_NUM_SCANCODES);

	// Mouse
	mState.Mouse.mPrevButtons = mState.Mouse.mCurrButtons;
	mState.Mouse.mIsRelative = false;
	mState.Mouse.mScrollWheel = Vector2::Zero;

	// Controller
	for (ControllerState cs : mState.Controllers)
	{
		memcpy(cs.mPrevButtons, cs.mCurrButtons, SDL_CONTROLLER_BUTTON_MAX);
	}
}

void InputSystem::Update()
{
	// Mouse
	int x = 0, y = 0;
	if (mState.Mouse.mIsRelative)
	{
		mState.Mouse.mCurrButtons = 
			SDL_GetRelativeMouseState(&x, &y);
	}
	else
	{
		mState.Mouse.mCurrButtons = 
			SDL_GetMouseState(&x, &y);
	}

	mState.Mouse.mMousePos.x = static_cast<float>(x);
	mState.Mouse.mMousePos.y = static_cast<float>(y);

	// Controller
	// Buttons
	for (int n = 0; n < 4; ++n)
	{
		if (mState.Controllers[n].mIsConnected)
		{
			for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; i++)
			{
				mState.Controllers[n].mCurrButtons[i] =
					SDL_GameControllerGetButton(mControllers[n],
						SDL_GameControllerButton(i));
			}

			// Triggers
			mState.Controllers[n].mLeftTrigger =
				Filter1D(SDL_GameControllerGetAxis(mControllers[n],
					SDL_CONTROLLER_AXIS_TRIGGERLEFT));
			mState.Controllers[n].mRightTrigger =
				Filter1D(SDL_GameControllerGetAxis(mControllers[n],
					SDL_CONTROLLER_AXIS_TRIGGERRIGHT));

			// Sticks
			x = SDL_GameControllerGetAxis(mControllers[n],
				SDL_CONTROLLER_AXIS_LEFTX);
			y = -SDL_GameControllerGetAxis(mControllers[n],
				SDL_CONTROLLER_AXIS_LEFTY);
			mState.Controllers[n].mLeftStick = Filter2D(x, y);

			x = SDL_GameControllerGetAxis(mControllers[n],
				SDL_CONTROLLER_AXIS_RIGHTX);
			y = -SDL_GameControllerGetAxis(mControllers[n],
				SDL_CONTROLLER_AXIS_RIGHTY);
			mState.Controllers[n].mRightStick = Filter2D(x, y);
		}
	}
}

void InputSystem::ProcessEvent(SDL_Event& event)
{
	switch (event.type)
	{
	case SDL_MOUSEWHEEL:
		mState.Mouse.mScrollWheel = Vector2(
			static_cast<float>(event.wheel.x),
			static_cast<float>(event.wheel.y));
		break;
	case SDL_CONTROLLERDEVICEADDED:
	case SDL_CONTROLLERDEVICEREMOVED:
		RefreshController();
		break;
	default:
		break;
	}
}

void InputSystem::RefreshController()
{
	for (int i = 0; i < 4; ++i)
	{
		mControllers[i] = nullptr;
	}
	for (int i = 0; i < 4; ++i)
	{
		if (SDL_IsGameController(i))
		{
			auto controller = SDL_GameControllerOpen(i);
			mControllers[i] = controller;
		}

		mState.Controllers[i].mIsConnected =
			(mControllers[i] != nullptr);
		memset(mState.Controllers[i].mCurrButtons,
			0, SDL_CONTROLLER_BUTTON_MAX);
		memset(mState.Controllers[i].mPrevButtons,
			0, SDL_CONTROLLER_BUTTON_MAX);
	}
}

void InputSystem::SetRelativeMouseMode(bool value)
{
	SDL_bool set = value ? SDL_TRUE : SDL_FALSE;
	SDL_SetRelativeMouseMode(set);

	mState.Mouse.mIsRelative = value;
}

float InputSystem::Filter1D(int input)
{
	// A value < dead zone is interpreted as 0%
	const int deadZone = 250;
	// A value > max value is interpreted as 100%
	const int maxValue = 30000;

	float retVal = 0.0f;

	// Take absolute value of input
	int absValue = input > 0 ? input : -input;
	// Ignore input within dead zone
	if (absValue > deadZone)
	{
		// Compute fractional value between dead zone and max value
		retVal = static_cast<float>(absValue - deadZone) /
		(maxValue - deadZone);
		// Make sure sign matches original value
		retVal = input > 0 ? retVal : -1.0f * retVal;
		// Clamp between -1.0f and 1.0f
		retVal = Math::Clamp(retVal, -1.0f, 1.0f);
	}

	return retVal;
}

Vector2 InputSystem::Filter2D(int inputX, int inputY)
{
	const float deadZone = 8000.0f;
	const float maxValue = 30000.0f;

	// Make into 2D vector
	Vector2 dir;
	dir.x = static_cast<float>(inputX);
	dir.y = static_cast<float>(inputY);

	float length = dir.Length();

	// If length < deadZone, should be no input
	if (length < deadZone)
	{
		dir = Vector2::Zero;
	}
	else
	{
		// Calculate fractional value between
		// dead zone and max value circles
		float f = (length - deadZone) / (maxValue - deadZone);
		// Clamp f between 0.0f and 1.0f
		f = Math::Clamp(f, 0.0f, 1.0f);
		// Normalize the vector, and then scale it to the
		// fractional value
		dir *= f / length;
	}

	return dir;
}
