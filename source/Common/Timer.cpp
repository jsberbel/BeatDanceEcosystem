/* ========================================================================
   File: Timer.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "Timer.h"

void Timer_Start(Timer* timer)
{
	timer->started = true;
	timer->paused = false;

	timer->startTicks = SDL_GetTicks();
	timer->pausedTicks = 0;
}

void Timer_Stop(Timer* timer)
{
	timer->started = false;
	timer->paused = false;

	timer->startTicks = 0;
	timer->pausedTicks = 0;
}

void Timer_Pause(Timer* timer)
{
	if (timer->started && !timer->paused)
	{
		timer->paused = true;

		timer->pausedTicks = SDL_GetTicks() - timer->startTicks;
		timer->startTicks = 0;
	}
}

void Timer_Unpause(Timer* timer)
{
	if (timer->started && timer->paused)
	{
		timer->paused = false;

		timer->startTicks = SDL_GetTicks() - timer->pausedTicks;
		timer->pausedTicks = 0;
	}
}

Uint32 Timer_GetTicks(Timer* timer)
{
	Uint32 currentTime = 0;

	if (timer->started)
	{
		if (timer->paused)
		{
			currentTime = timer->pausedTicks;
		}
		else
		{
			currentTime = SDL_GetTicks() - timer->startTicks;
		}
	}

	return currentTime;
}

inline bool Timer_IsStarted(Timer* timer)
{
	return (timer->started);
}

inline bool Timer_IsPaused(Timer* timer)
{
	return (timer->paused && timer->started);
}

