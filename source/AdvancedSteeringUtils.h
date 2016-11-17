/* ========================================================================
   File: AdvancedSteeringUtils.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

namespace AdvancedSteeringUtils
{
	// Returns a Force vector with Simple Path Following
	inline Vector2D DoSimplePathFollowing(Vector2D agentPosition, Vector2D agentSpeed,
		float agentMaxSpeed, float agentMaxForce, SimplePath* path, int& currentSegment, int& pathDirection, float pathArrivalDistance)
	{
		// Find Current segment to target
		Vector2D targetSegment = path->pathArray[currentSegment];

		// Are we near enough targetSegment
		if (Vector2D::Distance(agentPosition, targetSegment) < pathArrivalDistance)
		{
			// Update targetSegment next time
			currentSegment += pathDirection;
			if (currentSegment >= path->pathOccupation || currentSegment < 0)
			{
				// Loop
				pathDirection *= -1;
				currentSegment += pathDirection;
			}
		}

		// Seek target segment
		return SteeringUtils::DoSteeringSeek(targetSegment, agentPosition, agentSpeed, agentMaxSpeed, agentMaxForce);
	}

	// Returns a Force vector with Path Following
	inline Vector2D DoPathFollowing(Vector2D agentPosition, Vector2D agentSpeed,
		float agentMaxSpeed, float agentMaxForce, Path* path, bool& pathFollowingStarted,
		Vector2D& currentPathTarget, float& pathOffset, float pathArrivalDistance, bool loopPath)
	{
		if (pathFollowingStarted == false)
		{
			float targetParameter = Path_GetParameter(path, agentPosition, currentPathTarget);
			currentPathTarget = Path_GetPosition(path, targetParameter);
			pathFollowingStarted = true;
		}
		else if (Vector2D::Distance(agentPosition, currentPathTarget) < pathArrivalDistance)
		{
			float targetParameter = Path_GetParameter(path, agentPosition, currentPathTarget);
			targetParameter += pathOffset;

			if (targetParameter > 1.0f)
			{
				if (loopPath == true)
				{
					targetParameter = 1.0f;
					pathOffset *= -1.0f;
				}
				else
				{
					targetParameter = 1.0f;
				}
			}
			else if (targetParameter < 0.0f)
			{
				if (loopPath == true)
				{
					targetParameter = 0.0f;
					pathOffset *= -1.0f;
				}
				else
				{
					targetParameter = 0.0f;
				}
			}
			// Update Current Path Target
			currentPathTarget = Path_GetPosition(path, targetParameter);
		}

		return SteeringUtils::DoSteeringSeek(currentPathTarget, agentPosition, agentSpeed, agentMaxSpeed, agentMaxForce);
	}

	// Returns a Force vector with Collision Avoidance
	inline Vector2D DoCollisionAvoidance(Vector2D agentPosition, Vector2D agentSpeed, float agentAngle,
		float agentMaxSpeed, float agentMaxForce, Entity** targets, int numTargets,
		float coneHeight, float coneHalfAngle, bool& collisionDetected)
	{
		float shortestDistance = 999999999.9f;
		Entity* firstTarget = nullptr;

		Vector2D coneBasePoint = { agentPosition.x + coneHeight, agentPosition.y };
		Vector2D currentConeBase = Vector2DUtils::RotatePoint(agentPosition, coneBasePoint, agentAngle);

		collisionDetected = false;

		for (int i = 0; i < numTargets; ++i)
		{
			Entity* currentTarget = targets[i];

			if (Vector2DUtils::IsInsideCone(currentTarget->position, agentPosition, currentConeBase, coneHalfAngle))
			{
				float currentDistance = Vector2D::Distance(agentPosition, currentTarget->position);
				if (currentDistance < shortestDistance)
				{
					firstTarget = currentTarget;
					shortestDistance = currentDistance;
					collisionDetected = true;
				}
			}
		}

		if (firstTarget == nullptr)
		{
			return Vector2D();
		}

		// Flee from collision
		return SteeringUtils::DoSteeringFlee(firstTarget->position, agentPosition, agentSpeed, agentMaxSpeed, agentMaxForce);
	}
	
	// Returns a Force vector with Obstacle Avoidance
	inline Vector2D DoObstacleAvoidance(Vector2D agentPosition, Vector2D agentSpeed,
		float agentMaxSpeed, float agentMaxForce, float avoidanceLookahead, 
		float avoidanceDistance,  int obstacleArraySize, Obstacle* obstacleArray)
	{
		Vector2D raycastVector = agentPosition;
		raycastVector += Vector2D::Normalize(agentSpeed) * avoidanceLookahead;

		Vector2D intersectionPoint, normalVector;

		bool obstacleAvoidanceCollision = false;
		for (int i = 0; i < obstacleArraySize; ++i)
		{
			obstacleAvoidanceCollision = Obstacle_IntersectsSegment(&obstacleArray[i], agentPosition,
				raycastVector, intersectionPoint, normalVector);
			if (obstacleAvoidanceCollision == true)
			{
				break;
			}
		}

		Vector2D steeringForce;

		if (obstacleAvoidanceCollision == true)
		{
			// Seek where target = normal * avoidanceDistance
			Vector2D avoidTarget = intersectionPoint;
			avoidTarget += normalVector * avoidanceDistance;

			steeringForce = SteeringUtils::DoSteeringSeek(avoidTarget, agentPosition, agentSpeed, 
				agentMaxSpeed, agentMaxForce);
		}
		else
		{
			steeringForce = {};
		}

		return steeringForce;
	}

	// Returns a Force vector with Perimeter Avoidance
	inline Vector2D DoPerimeterAvoidance(Vector2D agentPosition, Vector2D agentSpeed,
		float agentMaxSpeed, float agentMaxForce, int perimeterWidth, int perimeterHeight, int perimeterBorder)
	{
		Vector2D steeringForce;
		Vector2D desiredVelocity = {};

		if (agentPosition.x < perimeterBorder)
		{
			desiredVelocity.x = agentMaxSpeed;
		}
		else if (agentPosition.x > perimeterWidth - perimeterBorder)
		{
			desiredVelocity.x = -agentMaxSpeed;
		}

		if (agentPosition.y < perimeterBorder)
		{
			desiredVelocity.y = agentMaxSpeed;
		}
		else if (agentPosition.y > perimeterHeight - perimeterBorder)
		{
			desiredVelocity.y = -agentMaxSpeed;
		}

		if (desiredVelocity.Length() > 0.0f)
		{
			// Calculate Steering Force
			steeringForce = desiredVelocity - agentSpeed;

			// Convert to Force
			// Divide by K_MAX_SPEED to get the speed factor
			steeringForce /= agentMaxSpeed;
			// Scale this factor by K_MAX_STEER_FORCE
			steeringForce *= agentMaxForce;
		}

		return steeringForce;
	}
}