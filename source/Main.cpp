/* ========================================================================
   File: Main.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "Common/GameFramework.h"
#include "EcosystemState.h"
#include "MenuState.h"
#include "FlowFieldState.h"

// Callback function called each frame from the StateManager loop
// Check currentStateCode to decide State Flow
bool SwitcherCallbackFunction(StateManager* manager, StateCode currentStateCode)
{
	bool quit = false;
	switch (currentStateCode)
	{
		case CONTINUE:
		{
			// Continue current state.
		} break;
		case QUIT:
		{
			// Quit current state
			quit = true;
		} break;
		case ECOSYSTEM:
		{
			BaseState* newState = new EcosystemState();
			newState->display = manager->display;
			StateManager_ChangeState(manager, newState);
		} break;
		case FLOW:
		{
			BaseState* newState = new FlowFieldState();
			newState->display = manager->display;
			StateManager_ChangeState(manager, newState);
		} break;
		case MENU:
		{
			BaseState* newState = new MenuState();
			newState->display = manager->display;
			StateManager_ChangeState(manager, newState);
		} break;
		default:
		{
			// Do nothing
		} break;
	}

	return quit;
}


// Entry Point
int main(int argc, char *argv[])
{
	// Init SDL
	InitializeApp();

	// Init Window
	Display display = {};
	Display_Initialize(&display, 
		FIXED_WIDTH, 
		FIXED_HEIGHT, 
		"Tècniques Intel·ligència Artificial per a Videojocs");

	// Init Gamepad Input
	InputSingleton::Instance()->Init();

	// Init StateManager
	StateManager manager = {};
	manager.display = &display;
	manager.switcherCallback = &SwitcherCallbackFunction;

	// Init State
	MenuState* menuState = new MenuState();
	menuState->display = &display;
	StateManager_ChangeState(&manager, menuState);

	// Loop
	StateManager_RunCurrentState(&manager);

	// Finish StateManager
	StateManager_Free(&manager);

	// Finish Gamepad Input
	InputSingleton::Instance()->Deinit();

	// Finish Window
	Display_Free(&display);

	// Finish SDL
	CloseApp();

	return 0;
}