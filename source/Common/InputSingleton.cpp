/* ========================================================================
   File: InputSingleton.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "InputSingleton.h"

void InputSingleton::Init()
{
	doQuit = false;
	mousePosition = {};
	mouseButtonState = false;
	mouseSecondaryButtonState = false;

	keyboardState = SDL_GetKeyboardState(&keyboardLength);
	oldKeyboardState = (Uint8*)SDL_malloc(keyboardLength);

	// Initialize Gamepad if present
	if (SDL_IsGameController(0))
	{
		gamepad = SDL_GameControllerOpen(0);
	}
}

void InputSingleton::Deinit()
{
	SDL_GameControllerClose(0);
}

void InputSingleton::Update()
{
	// Reset mouse 
	mouseButtonState = false;
	mouseSecondaryButtonState = false;

	// Keyboard Events
	SDL_memcpy(oldKeyboardState, keyboardState, keyboardLength);
	keyboardState = SDL_GetKeyboardState(NULL);

	// Input loop
	SDL_Event inputEvent = {};
	while (SDL_PollEvent(&inputEvent) != 0)
	{
		// Quit event
		if (inputEvent.type == SDL_QUIT)
		{
			doQuit = true;
		}
		// Mouse events
		else if (inputEvent.type == SDL_MOUSEMOTION)
		{
			mousePosition = { (float)inputEvent.motion.x, (float)inputEvent.motion.y };
		}
		else if (inputEvent.type == SDL_MOUSEBUTTONDOWN)
		{
			if (inputEvent.button.button == SDL_BUTTON_LEFT)
			{
				mouseButtonState = true;
			}
			else if (inputEvent.button.button == SDL_BUTTON_RIGHT)
			{
				mouseSecondaryButtonState = true;
			}
		}
	}

	// Gamepad state
	gamepadButtonsState.dPadUp.wasDown = gamepadButtonsState.dPadUp.isDown;
	gamepadButtonsState.dPadUp.isDown = SDL_GameControllerGetButton(gamepad, SDL_CONTROLLER_BUTTON_DPAD_UP);
	
	gamepadButtonsState.dPadDown.wasDown = gamepadButtonsState.dPadDown.isDown;
	gamepadButtonsState.dPadDown.isDown = SDL_GameControllerGetButton(gamepad, SDL_CONTROLLER_BUTTON_DPAD_DOWN);

	gamepadButtonsState.dPadLeft.wasDown = gamepadButtonsState.dPadLeft.isDown;
	gamepadButtonsState.dPadLeft.isDown = SDL_GameControllerGetButton(gamepad, SDL_CONTROLLER_BUTTON_DPAD_LEFT);

	gamepadButtonsState.dPadRight.wasDown = gamepadButtonsState.dPadRight.isDown;
	gamepadButtonsState.dPadRight.isDown = SDL_GameControllerGetButton(gamepad, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);

	gamepadButtonsState.start.wasDown = gamepadButtonsState.start.isDown;
	gamepadButtonsState.start.isDown = SDL_GameControllerGetButton(gamepad, SDL_CONTROLLER_BUTTON_START);

	gamepadButtonsState.select.wasDown = gamepadButtonsState.select.isDown;
	gamepadButtonsState.select.isDown = SDL_GameControllerGetButton(gamepad, SDL_CONTROLLER_BUTTON_BACK);
}

bool InputSingleton::IsKeyDown(SDL_Scancode key)
{
	return (keyboardState[key] == 1 && oldKeyboardState[key] == 0);
}

Uint8 InputSingleton::GetKey(SDL_Scancode key)
{
	return keyboardState[key];
}


