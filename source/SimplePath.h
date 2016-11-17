/* ========================================================================
   File: SimplePath.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Common/GameFramework.h"

#define MAX_POINTS_IN_PATH 50

struct SimplePath
{
	Vector2D pathArray[MAX_POINTS_IN_PATH];
	int pathOccupation = 0;
};

void SimplePath_Render(SimplePath* path, SDL_Renderer* renderer, Uint8 width, Uint32 pathColor, float arrivalRadius, Uint32	cColor);

void SimplePath_AddPoint(SimplePath* path, Vector2D point);
