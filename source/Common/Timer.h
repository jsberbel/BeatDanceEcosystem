/* ========================================================================
   File: Timer.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#if __APPLE__
#include <SDL2_image/SDL_image.h>
#else
#include <SDL_image.h>
#endif

struct Timer
{
    Uint32 startTicks = 0;
    Uint32 pausedTicks = 0;
    
    bool paused = false;
    bool started = false;
};

void Timer_Start(Timer* timer);
void Timer_Stop(Timer* timer);
void Timer_Pause(Timer* timer);
void Timer_Unpause(Timer* timer);
Uint32 Timer_GetTicks(Timer* timer);
inline bool Timer_IsStarted(Timer* timer);
inline bool Timer_IsPaused(Timer* timer);

