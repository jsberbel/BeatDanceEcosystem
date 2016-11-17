/* ========================================================================
   File: Obstacle.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Common/GameFramework.h"

#define MAX_POINTS_IN_PATH 50

struct Obstacle
{
	Vector2D position = {};
	float width = 0.0f;
	float height = 0.0f;
};

Obstacle Obstacle_Create(Vector2D position, float width, float height, float angle = 0.0f);
void Obstacle_Render(Obstacle* obstacle, SDL_Renderer* renderer, Uint32 color);
bool Obstacle_IntersectsSegment(Obstacle* obstacle, Vector2D segmentStart, Vector2D segmentEnd, Vector2D& intersectionPoint, Vector2D& normalVector);

