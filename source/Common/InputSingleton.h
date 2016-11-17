/* ========================================================================
   File: InputSingleton.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#if __APPLE__
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif

#include "Vector2D.h"

struct ButtonState
{
	Uint8 wasDown;
	Uint8 isDown;
};

struct GamepadState
{
	ButtonState dPadUp;
	ButtonState dPadDown;
	ButtonState dPadLeft;
	ButtonState dPadRight;
	ButtonState start;
	ButtonState select;
};

struct InputSingleton
{
	static InputSingleton* Instance()
	{
		static InputSingleton* instance = nullptr;
		if (instance == nullptr)
		{
			instance = (InputSingleton*)SDL_malloc(sizeof(InputSingleton));
		}

		return instance;
	}

	// System events
	bool doQuit;

	// Keyboard State
	int keyboardLength;
	Uint8 *oldKeyboardState;
	const Uint8 *keyboardState;

	// Mouse State
	Vector2D mousePosition;
	bool mouseButtonState;
	bool mouseSecondaryButtonState;

	// Gamepad handler
	// We will only handle dpad, start and select for now
	SDL_GameController* gamepad;
	GamepadState gamepadButtonsState;

	// Functions
	void Init();
	void Deinit();
	void Update();

	bool IsKeyDown(SDL_Scancode key);
	Uint8 GetKey(SDL_Scancode key);
};
