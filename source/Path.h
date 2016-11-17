/* ========================================================================
   File: Path.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Common/GameFramework.h"

#define MAX_POINTS_IN_PATH 50

struct Path
{
	Vector2D pathArray[MAX_POINTS_IN_PATH];
	int pathOccupation = 0;
};

void Path_Render(Path* path, SDL_Renderer* renderer, Uint8 width, Uint32 color);

void Path_AddPoint(Path* path, Vector2D point);

// Get Position in path from pathParameter [0, 1]
Vector2D Path_GetPosition(Path* path, float pathParameter);

// Get the parameter closest to position
float Path_GetParameter(Path* path, Vector2D position, Vector2D& outClosest);