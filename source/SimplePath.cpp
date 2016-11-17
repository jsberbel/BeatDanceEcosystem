/* ========================================================================
   File: SimplePath.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "SimplePath.h"

void SimplePath_Render(SimplePath* path, SDL_Renderer* renderer, Uint8 width, Uint32 pathColor, float arrivalRadius, Uint32	cColor)
{
	Vector2D startPosition;
	Vector2D endPosition;
	int pathCounter = path->pathOccupation;
	for (int i = 0; i < pathCounter-1; ++i)
	{
		startPosition = path->pathArray[i];
		endPosition = path->pathArray[i + 1];
		thickLineColor(renderer, startPosition.x, startPosition.y, endPosition.x, endPosition.y, width, pathColor);
		circleColor(renderer, startPosition.x, startPosition.y, arrivalRadius, cColor);
	}
	circleColor(renderer, endPosition.x, endPosition.y, arrivalRadius, cColor);
}

void SimplePath_AddPoint(SimplePath* path, Vector2D point)
{
	if (path->pathOccupation < MAX_POINTS_IN_PATH)
	{
		path->pathArray[path->pathOccupation] = point;
		++path->pathOccupation;
	}
	else
	{
		SDL_Log("Path reached max number of points.");
	}
}

