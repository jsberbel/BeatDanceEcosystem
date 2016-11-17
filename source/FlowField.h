/* ========================================================================
   File: FlowField.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Common/GameFramework.h"
#include "FlowObstacle.h"

#define FIELD_SIZE 32

struct FlowField
{
	Vector2D array[FIELD_SIZE][FIELD_SIZE];

	static Vector2D FlowField_GetCellCenter(Vector2D cellPosition)
	{
		Vector2D cellCenter = cellPosition;

		static int cellRealWidth = FIXED_WIDTH / FIELD_SIZE;
		static int cellRealHeight = FIXED_HEIGHT / FIELD_SIZE;

		cellCenter.x *= (float)cellRealWidth;
		cellCenter.x += (float)(cellRealWidth / 2);
		cellCenter.y *= (float)cellRealHeight;
		cellCenter.y += (float)(cellRealHeight / 2);

		return cellCenter;
	}
};

void FlowField_Init(FlowField* field);
void FlowField_AddObstacle(FlowField* field, FlowObstacle* obstacle);
void FlowField_Render(FlowField* field, SDL_Renderer* renderer, Uint32 color);
Vector2D FlowField_GetForceForPosition(FlowField* field, Vector2D position);
