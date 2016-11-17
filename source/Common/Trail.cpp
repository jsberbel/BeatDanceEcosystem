/* ========================================================================
   File: Trail.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "Trail.h"

#include "SDL_gfx/SDL2_gfxPrimitives.h"
#include "Utilities.h"

void Trail_Initialize(Trail* trail, int trailLength, int width, int height)
{
    trail->length = trailLength;
    trail->trailArray = (SDL_Point*)SDL_calloc(trail->length, sizeof(SDL_Point));
    trail->updatePoint = 0;
    trail->ocupation = 0;
    trail->width = width;
    trail->height = height;
}

void Trail_Free(Trail* trail)
{
    SDL_free(trail->trailArray);
}

void Trail_Update(Trail* trail, int x, int y)
{
    trail->trailArray[trail->updatePoint] = {x, y};
    ++trail->updatePoint;
    if (trail->updatePoint >= trail->length)
    {
        trail->updatePoint = 0;
    }
    ++trail->ocupation;
    trail->ocupation = IntegerUtils::Clamp(trail->ocupation, 0, trail->length);
}

void Trail_Render(Trail* trail, SDL_Renderer* renderer)
{
    int runningIndex = trail->updatePoint - 1;
    int numberUpdated = 0;
    if (runningIndex < 0)
    {
        runningIndex = trail->length - 1;
    }
    float scale = 1.0f;
    while(numberUpdated < trail->ocupation)
    {
        boxRGBA(renderer, trail->trailArray[runningIndex].x - (int)ceil((float)trail->width * scale),
                trail->trailArray[runningIndex].y - (int)ceil((float)trail->width * scale),
                trail->trailArray[runningIndex].x + (int)round((float)trail->width * scale),
                trail->trailArray[runningIndex].y + (int)round((float)trail->height * scale),
                255, 255, 0, 255);
        
        scale -= 1.0f/ (float)trail->ocupation;
        if(scale <= 0.0f)
        {
            break;
        }
        
        ++numberUpdated;
        --runningIndex;
        if (runningIndex < 0)
        {
            runningIndex = trail->length - 1;
        }
    }
}