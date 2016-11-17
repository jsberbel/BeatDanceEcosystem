/* ========================================================================
   File: Trail.h
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

struct Trail
{
    SDL_Point* trailArray = nullptr;
    int length = 0;
    int updatePoint = 0;
    int ocupation = 0;
    
    int width = 0;
    int height = 0;
};

void Trail_Initialize(Trail* trail, int trailLength, int width, int height);
void Trail_Free(Trail* trail);
void Trail_Update(Trail* trail, int x, int y);
void Trail_Render(Trail* trail, SDL_Renderer* renderer);
