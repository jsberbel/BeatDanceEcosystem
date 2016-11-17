/* ========================================================================
   File: Obstacle.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "Obstacle.h"

Obstacle Obstacle_Create(Vector2D position, float width, float height, float angle)
{
	Obstacle createdObstacle = {};
	createdObstacle.position = position;
	createdObstacle.width = width;
	createdObstacle.height = height;
	return createdObstacle;
}

void Obstacle_Render(Obstacle* obstacle, SDL_Renderer* renderer, Uint32 color)
{
	boxColor(renderer, obstacle->position.x, obstacle->position.y,
		obstacle->position.x + obstacle->width, obstacle->position.y + obstacle->height, color);
}

bool Obstacle_IntersectsSegment(Obstacle* obstacle, Vector2D segmentStart, Vector2D segmentEnd, Vector2D& intersectionPoint, Vector2D& normalVector)
{
	// Check if segment intersects any of the 4 lines that comprise the wall
	Vector2D obstacleEndSegment = obstacle->position;
	obstacleEndSegment += {obstacle->width, 0.0f};
	bool result = Vector2DUtils::SegmentSegmentIntersection(obstacle->position, obstacleEndSegment, segmentStart, segmentEnd, intersectionPoint);

	float dx = obstacleEndSegment.x - obstacle->position.x;
	float dy = obstacleEndSegment.y - obstacle->position.y;

	if (segmentStart.y < obstacle->position.y)
	{
		normalVector = { dy, -dx };
	}
	else
	{
		normalVector = { -dy, dx };
	}
	normalVector.Normalize();
	return result;

}

