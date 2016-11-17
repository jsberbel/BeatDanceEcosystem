/* ========================================================================
   File: SteeringUtils.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Common/GameFramework.h"

namespace SteeringUtils
{
	// Returns a Velocity vector with Seek behavior 
	inline Vector2D DoKinematicSeek(Vector2D targetPosition, Vector2D agentPosition, float agentMaxSpeed)
	{
		// Calculate desired velocity
		Vector2D desiredVelocity = targetPosition - agentPosition;
		// Normalize
		desiredVelocity.Normalize();
		// Scale by agentMaxSpeed
		desiredVelocity *= agentMaxSpeed;

		return desiredVelocity;
	}

	// Returns a Velocity vector with Flee behavior 
	inline Vector2D DoKinematicFlee(Vector2D targetPosition, Vector2D agentPosition, float agentMaxSpeed)
	{
		// Calculate desired velocity
		Vector2D desiredVelocity = agentPosition - targetPosition;
		// Normalize
		desiredVelocity.Normalize();
		// Scale by agentMaxSpeed
		desiredVelocity *= agentMaxSpeed;

		return desiredVelocity;
	}

	// Returns a Force vector with Seek behavior 
	inline Vector2D DoSteeringSeek(Vector2D targetPosition, Vector2D agentPosition, Vector2D agentSpeed, 
		float agentMaxSpeed, float agentMaxForce)
	{
		// Calculate desired velocity
		Vector2D desiredVelocity = DoKinematicSeek(targetPosition, agentPosition, agentMaxSpeed);

		// Calculate Steering Force
		Vector2D steeringForce = desiredVelocity - agentSpeed;

		// Divide by agentMaxSpeed to get the speed factor
		steeringForce /= agentMaxSpeed;
		// Scale this factor by agentMaxForce
		steeringForce *= agentMaxForce;

		return steeringForce;
	}

	// Returns a Force vector with Flee behavior 
	inline Vector2D DoSteeringFlee(Vector2D targetPosition, Vector2D agentPosition, Vector2D agentSpeed,
		float agentMaxSpeed, float agentMaxForce)
	{
		// Calculate desired velocity
		Vector2D desiredVelocity = DoKinematicFlee(targetPosition, agentPosition, agentMaxSpeed);

		// Calculate Steering Force
		Vector2D steeringForce = desiredVelocity - agentSpeed;
		
		// Divide by agentMaxSpeed to get the speed factor
		steeringForce /= agentMaxSpeed;
		// Scale this factor by agentMaxForce
		steeringForce *= agentMaxForce;

		return steeringForce;
	}

	// Returns a Force vector with Arrive behavior 
	inline Vector2D DoSteeringArrive(Vector2D targetPosition, Vector2D agentPosition, Vector2D agentSpeed,
		float agentMaxSpeed, float agentMaxForce, float slowingRadius)
	{
		// Calculate desired velocity
		Vector2D desiredVelocity = targetPosition - agentPosition;

		// Are we inside slowing radius?
		float distanceToTarget = desiredVelocity.Length();

		float targetSpeed;
		if (distanceToTarget > slowingRadius)
		{
			// Scale by K_MAX_SPEED
			targetSpeed = agentMaxSpeed;
		}
		else
		{
			// Slow down based on distance to target
			targetSpeed = agentMaxSpeed * (distanceToTarget / slowingRadius);
		}

		// Normalize and scale by targetSpeed
		desiredVelocity.Normalize();
		desiredVelocity *= targetSpeed;

		// Calculate Steering Force
		Vector2D steeringForce = desiredVelocity - agentSpeed;

		// Divide by agentMaxSpeed to get the speed factor
		steeringForce /= agentMaxSpeed;
		// Scale this factor by agentMaxForce
		steeringForce *= agentMaxForce;

		return steeringForce;
	}

	inline Vector2D CalculateFuturePosition(Vector2D targetPosition, Vector2D targetSpeed,
		Vector2D agentPosition, Vector2D agentSpeed, float maximumLookAhead)
	{
		// Calculate desired velocity
		Vector2D distanceVector = targetPosition - agentPosition;
		float distanceToTarget = distanceVector.Length();

		// Calculate lookAhead time
		float currentSpeed = agentSpeed.Length();

		// If speed is to small or distance to big to give a reasonable 
		// look ahead time to target, use maximumLookAhead
		float timeLookAhead = distanceToTarget / currentSpeed;
		if (timeLookAhead > maximumLookAhead)
		{
			timeLookAhead = maximumLookAhead;
		}

		// Calculate future position based on current position and speed
		Vector2D futurePosition = targetPosition + (targetSpeed * timeLookAhead);

		return futurePosition;
	}

	// Returns a Force vector with Pursue behavior 
	inline Vector2D DoSteeringPursue(Vector2D targetPosition, Vector2D targetSpeed, Vector2D agentPosition, Vector2D agentSpeed,
		float agentMaxSpeed, float agentMaxForce, float maximumLookAhead)
	{
		Vector2D futurePosition = CalculateFuturePosition(targetPosition, targetSpeed, agentPosition, agentSpeed, maximumLookAhead);
		Vector2D steeringForce = DoSteeringSeek(futurePosition, agentPosition, agentSpeed, agentMaxSpeed, agentMaxForce);

		return steeringForce;
	}

	// Returns a Force vector with Evade behavior 
	inline Vector2D DoSteeringEvade(Vector2D targetPosition, Vector2D targetSpeed, Vector2D agentPosition, Vector2D agentSpeed,
		float agentMaxSpeed, float agentMaxForce, float maximumLookAhead)
	{
		Vector2D futurePosition = CalculateFuturePosition(targetPosition, targetSpeed, agentPosition, agentSpeed, maximumLookAhead);
		Vector2D steeringForce = DoSteeringFlee(futurePosition, agentPosition, agentSpeed, agentMaxSpeed, agentMaxForce);

		return steeringForce;
	}

	inline Vector2D DoSteeringWander(Vector2D agentPosition, Vector2D agentSpeed, float agentMaxSpeed, float agentMaxForce,
		float angle, float* wanderAnglePointer, int wanderMaxChange,
		int wanderCircleOffset, int wanderCircleRadius)
	{
		float wanderAngle = *wanderAnglePointer;
		
		// Update wanderAngle
		wanderAngle += FloatUtils::RandomBinomial() * wanderMaxChange;
		float targetAngle = angle + wanderAngle;

		// Calculate the center of the wander circle
		Vector2D speedOrientationVector = Vector2D::Normalize(agentSpeed);
		Vector2D wanderCircleCenter = agentPosition + speedOrientationVector * wanderCircleOffset;

		Vector2D wanderDisplacementVector;
		// Calculate and add displacement location (Using the Parametric equation of the circle)
		wanderDisplacementVector.x = wanderCircleRadius * cosf(targetAngle * DEG2RAD);
		wanderDisplacementVector.y = wanderCircleRadius * sinf(targetAngle * DEG2RAD);
		wanderDisplacementVector += wanderCircleCenter;

		Vector2D steeringForce = SteeringUtils::DoSteeringSeek(wanderDisplacementVector, agentPosition, agentSpeed, agentMaxSpeed, agentMaxForce);

		// Change wanderAngle pointed value
		*wanderAnglePointer = wanderAngle;

		return steeringForce;
	}
}