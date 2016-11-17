/* ========================================================================
   File: FlowObstacle.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "FlowObstacle.h"

void FlowObstacle_Init(FlowObstacle* obstacle, Vector2D position, float radius)
{
	obstacle->position = position;
	obstacle->radius = radius;
}

void FlowObstacle_Render(FlowObstacle* obstacle, SDL_Renderer* renderer, Uint32 color)
{
	filledCircleColor(renderer, obstacle->position.x, obstacle->position.y, obstacle->radius, color);
}