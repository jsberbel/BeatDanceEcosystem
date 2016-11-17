/* ========================================================================
   File: StateManager.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "StateManager.h"

#include "Timer.h"
#include "InputSingleton.h"

#define CHECK_PERFORMANCE 0

void StateManager_Free(StateManager* manager)
{
	if (manager->currentState != nullptr)
	{
		manager->currentState->Deinit();
		delete manager->currentState;
		manager->currentState = nullptr;
	}
}

void StateManager_ChangeState(StateManager* manager, BaseState* gameState)
{
	if (manager->currentState != nullptr)
	{
		manager->currentState->Deinit();
		delete manager->currentState;
		manager->currentState = nullptr;
	}
	manager->currentState = gameState;
	manager->currentState->Init();
}

void StateManager_RunCurrentState(StateManager* manager)
{
#if CHECK_PERFORMANCE
	Uint64 start, now, frameCounter = 0;
	double frameInMs, totalTimeInSeconds = 0.0;
#endif
	
	Timer stepTimer;
	Timer_Start(&stepTimer);
	
	bool quit = false;
	while (!quit)
	{
#if CHECK_PERFORMANCE
		start = SDL_GetPerformanceCounter();
#endif
		InputSingleton::Instance()->Update();

		StateCode currentStateCode = manager->currentState->HandleInput();
		quit = manager->switcherCallback(manager, currentStateCode);
		
		float timeStep = Timer_GetTicks(&stepTimer) / 1000.0f;
		manager->currentState->Update(timeStep);
		Timer_Start(&stepTimer);
		
		// Allow the OS to sleep the process
		SDL_Delay(1);
		
		// Render the screen
		if (manager->display)
		{
			Display_Clear(manager->display);
			manager->currentState->Render();            
			Display_RefreshWindow(manager->display);
		}
		
#if CHECK_PERFORMANCE
		now = SDL_GetPerformanceCounter();
		frameInMs = (double)((now - start) * 1000) / SDL_GetPerformanceFrequency();
		SDL_Log("Time per frame = %f ms\n", frameInMs);
		totalTimeInSeconds += (frameInMs / 1000);
		++frameCounter;
		SDL_Log("Frames per second = %f\n", frameCounter / totalTimeInSeconds);
#endif
	}
}


