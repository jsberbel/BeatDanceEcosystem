/* ========================================================================
   File: FlockingUtils.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

namespace FlockingUtils
{
	inline Vector2D DoFlocking(int* boidArraySize, Boid* boidArray, Boid* agent, Vector2D agentPosition, 
		float agentMaxForce, float flockNeighbourhoodRadius, float alignmentWeight, float cohesionWeight, float separationWeight)
	{
		Vector2D alignment = {};
		Vector2D cohesion = {};
		Vector2D separation = {};

		int neighbourCount = 0;
		int boidSize = *boidArraySize;

		for (int i = 0; i < boidSize; ++i)
		{
			if (&boidArray[i] == agent)
			{
				// Do not count myself
				//SDL_Log("I found me!");
			}
			else
			{
				// Is neighbour?
				if (Vector2D::Distance(agentPosition, boidArray[i].position) < flockNeighbourhoodRadius)
				{
					alignment += boidArray[i].speed;
					cohesion += boidArray[i].position;
					separation += agentPosition - boidArray[i].position;
					++neighbourCount;
				}
			}
		}

		if (neighbourCount != 0)
		{
			// Alignment Calculation
			alignment /= neighbourCount;
			// Normalize
			alignment.Normalize();

			// Cohesion Calculation
			cohesion /= neighbourCount;
			cohesion -= agentPosition;
			// Normalize
			cohesion.Normalize();

			// Separation Calculation
			separation /= neighbourCount;
			// Normalize
			separation.Normalize();
		}

		// Add all forces together
		Vector2D steeringFoces[3] = { alignment, cohesion, separation };
		float steeringWeights[3] = { alignmentWeight, cohesionWeight, separationWeight };
		Vector2D flockingForce = CombiningSteering::DoWeightedBlending(3, steeringFoces, steeringWeights);

		// Convert to Force
		flockingForce.Normalize();
		flockingForce *= agentMaxForce;

		return flockingForce;
	}
}