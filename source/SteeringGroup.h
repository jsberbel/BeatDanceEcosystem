/* ========================================================================
   File: SteeringGroup.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Common/GameFramework.h"

struct SteeringGroup
{
	int size = 0;
	float* weightArray = nullptr;
	Vector2D* calculatedForces = nullptr;
};

void SteeringGroup_Initialize(SteeringGroup* group, int size);
void SteeringGroup_Free(SteeringGroup* group);
Vector2D SteeringGroup_CalculateSteering(SteeringGroup* group);