/* ========================================================================
   File: Path.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "Path.h"

void Path_Render(Path* path, SDL_Renderer* renderer, Uint8 width, Uint32 color)
{
	int pathCounter = path->pathOccupation;
	for (int i = 0; i < pathCounter-1; ++i)
	{
		Vector2D startPosition = path->pathArray[i];
		Vector2D endPosition = path->pathArray[i + 1];
		thickLineColor(renderer, startPosition.x, startPosition.y, endPosition.x, endPosition.y, width, color);
	}
}

void Path_AddPoint(Path* path, Vector2D point)
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

// Get Position in path from pathParameter [0, 1]
Vector2D Path_GetPosition(Path* path, float pathParameter)
{
	// Get which segment is closest to pathParameter
	float nearestPointDecimal = (path->pathOccupation-1) * pathParameter;
	int nearestPointInteger = floor(nearestPointDecimal);
	nearestPointDecimal -= nearestPointInteger;

	// Get position inside this segment
	Vector2D resultPoint = path->pathArray[nearestPointInteger];
	Vector2D nextSegment = {};

	if (nearestPointInteger != path->pathOccupation - 1)
	{
		nextSegment = path->pathArray[nearestPointInteger + 1] - path->pathArray[nearestPointInteger];
	}

	resultPoint.x += nextSegment.x * nearestPointDecimal;
	resultPoint.y += nextSegment.y * nearestPointDecimal;

	return resultPoint;
}


// Get the parameter closest to position
float Path_GetParameter(Path* path, Vector2D position, Vector2D& outClosest)
{
	float closestDistance = 999999999.9f;
	float nearestPointInteger = 0.0f;
	Vector2D closestPoint = {};

	int pathMax = path->pathOccupation;
	// Find Closest Point to position for each segment and store the smallest
	for (int i = 0; i < pathMax - 1; ++i)
	{
		Vector2D startPosition = path->pathArray[i];
		Vector2D endPosition = path->pathArray[i + 1];

		Vector2D currentCandidate = Vector2DUtils::GetClosestPoint(startPosition, endPosition, position);
		float distance = Vector2D::Distance(position, currentCandidate);
		
		if (distance < closestDistance)
		{
			closestPoint = currentCandidate;
			closestDistance = distance;
			nearestPointInteger = (float)i;
		}
	}

	// Take out value
	outClosest = closestPoint;

	Vector2D intermediateFullSegment = path->pathArray[(int)nearestPointInteger + 1]
									- path->pathArray[(int)nearestPointInteger];
	Vector2D intermediateCurrent = closestPoint - path->pathArray[(int)nearestPointInteger];
	
	// Calculate Decimal part of parameter
	float nearestPointDecimal = intermediateCurrent.Length() / intermediateFullSegment.Length();
	// Calculate Parameter
	float resultParameter = (nearestPointInteger + nearestPointDecimal) / (path->pathOccupation - 1);
	return resultParameter;
}
