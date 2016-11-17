/* ========================================================================
   File: CombiningSteeringUtils.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "SteeringGroup.h"

namespace CombiningSteering
{
	inline Vector2D DoWeightedBlending(int size, Vector2D* steeringForceArray, float* weightArray)
	{
		Vector2D resultingForce = {};
		for (int i = 0; i < size; ++i)
		{
			resultingForce += steeringForceArray[i] * weightArray[i];
		}

		return resultingForce;
	}

	inline Vector2D DoPriorityBlending(int groupsSize, SteeringGroup* groupsArray, float epsilonValue, int& usedGroup)
	{
		Vector2D steeringForce = {};

		for (int i = 0; i < groupsSize; ++i)
		{
			steeringForce = SteeringGroup_CalculateSteering(&groupsArray[i]);
			if (steeringForce.Length() > epsilonValue)
			{
				usedGroup = i; // Debug purposes
				break;
			}
		}

		return steeringForce;
	}
}
