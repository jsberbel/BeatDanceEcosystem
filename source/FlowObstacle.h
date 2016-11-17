/* ========================================================================
   File: FlowObstacle.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Common/GameFramework.h"

struct FlowObstacle
{
	Vector2D position;
	float radius;
};

void FlowObstacle_Init(FlowObstacle* obstacle, Vector2D position, float radius);
void FlowObstacle_Render(FlowObstacle* obstacle, SDL_Renderer* renderer, Uint32 color);
