/* ========================================================================
   File: FlowField.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */


#include "FlowField.h"

void FlowField_Init(FlowField* field)
{
	for (int i = 0; i < FIELD_SIZE; ++i)
	{
		for (int j = 0; j < FIELD_SIZE; ++j)
		{
			field->array[i][j] = Vector2D();
		}
	}
}

void FlowField_AddObstacle(FlowField* field, FlowObstacle* obstacle)
{
	int cellSize;
	// Get smallest cell size (width vs height)
	if (FIXED_WIDTH < FIXED_HEIGHT)
	{
		cellSize = FIXED_WIDTH / FIELD_SIZE;
	}
	else
	{
		cellSize = FIXED_HEIGHT / FIELD_SIZE;
	}

	// Calculate affected radius for obstacle
	float affectedGridRadius = 2.0f + ceilf(obstacle->radius / cellSize);

	int obstacleX = (obstacle->position.x / (FIXED_WIDTH / FIELD_SIZE));
	int obstacleY = (obstacle->position.y / (FIXED_HEIGHT / FIELD_SIZE));

	for (int i = -affectedGridRadius; i <= affectedGridRadius; ++i)
	{
		for (int j = -affectedGridRadius; j <= affectedGridRadius; ++j)
		{
			int currentX = obstacleX + i;
			int currentY = obstacleY + j;

			// Clamp inside Grid
			if (currentX >= 0 && currentX < FIELD_SIZE &&
				currentY >= 0 && currentY < FIELD_SIZE)
			{
				Vector2D cellCenter = FlowField::FlowField_GetCellCenter({ (float)currentX , (float)currentY });
				Vector2D flowVector = cellCenter - obstacle->position;
				flowVector.Normalize();
				flowVector *= (cellSize / Vector2D::Distance(cellCenter, obstacle->position));

				field->array[currentX][currentY] += flowVector;
			}
		}
	}
}

void FlowField_Render(FlowField* field, SDL_Renderer* renderer, Uint32 color)
{
	// Render Grid
	int horizontalStep = FIXED_WIDTH / FIELD_SIZE;
	int verticalStep = FIXED_HEIGHT / FIELD_SIZE;

	for (int i = 0; i <= FIXED_WIDTH; i += horizontalStep)
	{
		lineColor(renderer, i, 0, i, FIXED_HEIGHT, color);
	}

	for (int j = 0; j < FIXED_HEIGHT; j += verticalStep)
	{
		lineColor(renderer, 0, j, FIXED_WIDTH, j, color);
	}

	// Render Flow Forces
	static float cellHalfWidth = FIXED_WIDTH / FIELD_SIZE / 2.0f;
	static float cellHalfHeight = FIXED_HEIGHT / FIELD_SIZE / 2.0f;

	for (int i = 0; i < FIELD_SIZE; ++i)
	{
		for (int j = 0; j < FIELD_SIZE; ++j)
		{
			if (field->array[i][j].Length() > 0.0f)
			{
				Vector2D cellCenter = FlowField::FlowField_GetCellCenter({ (float)i, (float)j });
				lineColor(renderer, cellCenter.x, cellCenter.y,
					cellCenter.x + field->array[i][j].x * cellHalfWidth,
					cellCenter.y + field->array[i][j].y * cellHalfHeight, Colors::ALIZARIN);
			}
		}
	}
}

Vector2D FlowField_GetForceForPosition(FlowField* field, Vector2D position)
{
	static int cellRealWidth = FIXED_WIDTH / FIELD_SIZE;
	static int cellRealHeight = FIXED_HEIGHT / FIELD_SIZE;

	int gridPositionX = (int)floorf(position.x / cellRealWidth);
	int gridPositionY = (int)floorf(position.y / cellRealHeight);

	return field->array[gridPositionX][gridPositionY];
}