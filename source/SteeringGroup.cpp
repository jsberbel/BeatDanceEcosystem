/* ========================================================================
   File: SteeringGroup.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "SteeringGroup.h"

#include "CombiningSteeringUtils.h"

void SteeringGroup_Initialize(SteeringGroup* group, int size)
{
	group->weightArray = (float*)SDL_calloc(size, sizeof(float));
	group->calculatedForces = (Vector2D*)SDL_calloc(size, sizeof(Vector2D));
	group->size = size;
}

void SteeringGroup_Free(SteeringGroup* group)
{
	SDL_free(group->weightArray);
	SDL_free(group->calculatedForces);
}

Vector2D SteeringGroup_CalculateSteering(SteeringGroup* group)
{
	Vector2D totalForce = CombiningSteering::DoWeightedBlending(group->size, group->calculatedForces, group->weightArray);
	return totalForce;
}