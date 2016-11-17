/* ========================================================================
   File: Boid.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "Boid.h"

#include "SteeringUtils.h"
#include "AdvancedSteeringUtils.h"
#include "FlockingUtils.h"

void Boid::SetBehaviour(Behaviour newBehaviour)
{
	currentBehaviour = newBehaviour;
	if (currentBehaviour == Behaviour::SEEK_KINEMATIC || currentBehaviour == Behaviour::FLEE_KINEMATIC
		|| currentBehaviour == Behaviour::SIMPLE_WANDER)
	{
		isSteering = false;
	}
	else
	{
		isSteering = true;
	}
}

void Boid::Update(float deltaTime)
{
	switch (currentBehaviour) {
	case Behaviour::SEEK_KINEMATIC:
		DoKinematicSeek();
		break;
	case Behaviour::SEEK_STEERING:
		DoSteeringSeek(deltaTime);
		break;
	case Behaviour::FLEE_KINEMATIC:
		DoKinematicFlee();
		break;
	case Behaviour::FLEE_STEERING:
		DoSteeringFlee(deltaTime);
		break;
	case Behaviour::ARRIVE:
		DoArrive(deltaTime);
		break;
	case Behaviour::PURSUE:
		DoPursue(deltaTime);
		break;
	case Behaviour::EVADE:
		DoEvade(deltaTime);
		break;
	case Behaviour::SIMPLE_WANDER:
		DoSimpleWander(deltaTime);
		break;
	case Behaviour::WANDER:
		DoFullWander(deltaTime);
		break;
	case Behaviour::SIMPLE_PATH_FOLLOWING:
		DoSimplePathFollowing(deltaTime);
		break;
	case Behaviour::PATH_FOLLOWING:
		DoPathFollowing(deltaTime);
		break;
	case Behaviour::OBSTACLE_AVOIDANCE:
		// Mix both to spice it up!
		DoObstacleAvoidance(deltaTime);
		if (obstacleAvoidanceCollision == false)
		{
			DoSteeringSeek(deltaTime);
		}
		break;
	case Behaviour::COLLISION_AVOIDANCE:
		// Mix both to spice it up!
		DoCollisionAvoidance(deltaTime);
		DoSteeringSeek(deltaTime);
		//DoSimplePathFollowing(deltaTime);
		break;
	case Behaviour::PRIORITY_OBSTACLE:
		DoPriorityObstacleSteering(deltaTime);
		break;
	case Behaviour::PRIORITY_COLLISION:
		DoPriorityCollisionSteering(deltaTime);
		break;
	case Behaviour::FLOCKING:
		DoFlocking(deltaTime);
		DoPerimeterAvoidance(deltaTime);
		break;
	case Behaviour::FLOCKING_SEEK:
		DoSteeringSeek(deltaTime);
		DoFlocking(deltaTime);
		DoPerimeterAvoidance(deltaTime);
		break;
	case Behaviour::FLOW_FLOCK_SEEK_PRIORITY:
		//DoSteeringSeek(deltaTime);
		//DoFlocking(deltaTime);
		//DoPerimeterAvoidance(deltaTime);
		//DoFlowFieldSteering(deltaTime);
		DoPriorityFlowFlockingSeek(deltaTime);
		break;
	default:
		break;
	}

	// Call base Update
	Entity::Update(deltaTime);

	// Wrap player
	Utils::DoWrap(position);
}

void Boid::DoKinematicSeek()
{
	desiredVelocity = SteeringUtils::DoKinematicSeek(target->position, position, K_MAX_SPEED);
	speed = desiredVelocity;
}

void Boid::DoKinematicFlee()
{
	desiredVelocity = SteeringUtils::DoKinematicFlee(target->position, position, K_MAX_SPEED);
	speed = desiredVelocity;
}

void Boid::DoSteeringSeek(float deltaTime)
{
#if DEBUG
	// Calculate desired velocity
	desiredVelocity = target->position - position;

	// Normalize and Scale by K_MAX_SPEED
	desiredVelocity.Normalize();
	desiredVelocity *= K_MAX_SPEED;

	// Calculate Steering Force
	steeringForce = desiredVelocity - speed;
	
	// Convert to Force
	// Divide by K_MAX_SPEED to get the speed factor
	steeringForce /= K_MAX_SPEED;
	// Scale this factor by K_MAX_STEER_FORCE
	steeringForce *= K_MAX_STEER_FORCE;
#else
	steeringForce = SteeringUtils::DoSteeringSeek(target->position, position, speed, K_MAX_SPEED, K_MAX_STEER_FORCE);
#endif

	// Modify according to mass to get acceleration
	acceleration = steeringForce / mass;

	// Update Speed with acceleration
	speed += acceleration * deltaTime;
}

void Boid::DoSteeringFlee(float deltaTime)
{
#if DEBUG
	// Calculate desired velocity
	desiredVelocity = position - target->position;

	// Normalize and Scale by K_MAX_SPEED
	desiredVelocity.Normalize();
	desiredVelocity *= K_MAX_SPEED;

	// Calculate Steering Force
	steeringForce = desiredVelocity - speed;
	
	// Convert to Force
	// Divide by K_MAX_SPEED to get the speed factor
	steeringForce /= K_MAX_SPEED;
	// Scale this factor by K_MAX_STEER_FORCE
	steeringForce *= K_MAX_STEER_FORCE;
#else
	steeringForce = SteeringUtils::DoSteeringFlee(target->position, position, speed, K_MAX_SPEED, K_MAX_STEER_FORCE);
#endif

	// Modify according to mass to get acceleration
	acceleration = steeringForce / mass;

	// Update Speed with acceleration
	speed += acceleration * deltaTime;
}

void Boid::DoArrive(float deltaTime)
{
#if DEBUG
	// Calculate desired velocity
	desiredVelocity = target->position - position;
	// Are we inside slowing radius?
	float distanceToTarget = desiredVelocity.Length();

	float targetSpeed;
	if (distanceToTarget > slowingRadius)
	{
		// Scale by K_MAX_SPEED
		targetSpeed = K_MAX_SPEED;
	}
	else
	{
		// Slow down based on distance to target
		targetSpeed = K_MAX_SPEED * (distanceToTarget / slowingRadius);
	}

	// Normalize and scale by targetSpeed
	desiredVelocity.Normalize();
	desiredVelocity *= targetSpeed;

	// Update Steering Velocity
	steeringForce = desiredVelocity - speed;

	// Convert to Force
	// Divide by K_MAX_SPEED to get the speed factor
	steeringForce /= K_MAX_SPEED;
	// Scale this factor by K_MAX_STEER_FORCE
	steeringForce *= K_MAX_STEER_FORCE;
#else
	steeringForce = SteeringUtils::DoSteeringArrive(target->position, position, speed, 
		K_MAX_SPEED, K_MAX_STEER_FORCE, slowingRadius);
#endif

	// Modify according to mass to get acceleration
	acceleration = steeringForce / mass;

	// Update Speed with acceleration
	speed += acceleration * deltaTime;
}

void Boid::DoPursue(float deltaTime)
{
#if DEBUG
	// Calculate desired velocity
	Vector2D distanceVector = target->position - position;
	float distanceToTarget = distanceVector.Length();

	// Calculate lookAhead time
	float currentSpeed = speed.Length();

	// If speed is to small or distance to big to give a reasonable 
	// look ahead time to target, use maximumLookAhead
	timeLookAhead = distanceToTarget / currentSpeed;
	if (timeLookAhead > maximumLookAhead)
	{
		timeLookAhead = maximumLookAhead;
	}

	// Calculate future position based on current position and speed
	futurePosition = target->position + (target->speed * timeLookAhead);

	// Calculate desired velocity
	desiredVelocity = futurePosition - position;

	// Normalize
	desiredVelocity.Normalize();
	// Scale by K_MAX_SPEED
	desiredVelocity *= K_MAX_SPEED;

	// Update Steering Velocity
	steeringForce = desiredVelocity - speed;

	// Convert to Force
	// Divide by K_MAX_SPEED to get the speed factor
	steeringForce /= K_MAX_SPEED;
	// Scale this factor by K_MAX_STEER_FORCE
	steeringForce *= K_MAX_STEER_FORCE;
#else
	steeringForce = SteeringUtils::DoSteeringPursue(target->position, target->speed, position, speed,
		K_MAX_SPEED, K_MAX_STEER_FORCE, maximumLookAhead);
#endif

	// Modify according to mass to get acceleration
	acceleration = steeringForce / mass;

	// Update Speed with acceleration
	speed += acceleration * deltaTime;
}

void Boid::DoEvade(float deltaTime)
{
#if DEBUG
	// Calculate desired velocity
	Vector2D distanceVector = target->position - position;
	float distanceToTarget = distanceVector.Length();

	// Calculate lookAhead time
	float currentSpeed = speed.Length();

	// If speed is to small or distance to big to give a reasonable 
	// look ahead time to target, use maximumLookAhead
	timeLookAhead = distanceToTarget / currentSpeed;
	if (timeLookAhead > maximumLookAhead)
	{
		timeLookAhead = maximumLookAhead;
	}

	// Calculate future position based on current position and speed
	futurePosition = target->position + (target->speed * timeLookAhead);

	// Calculate desired velocity
	desiredVelocity = position - futurePosition;

	// Normalize
	desiredVelocity.Normalize();
	// Scale by K_MAX_SPEED
	desiredVelocity *= K_MAX_SPEED;

	// Update Steering Velocity
	steeringForce = desiredVelocity - speed;

	// Convert to Force
	// Divide by K_MAX_SPEED to get the speed factor
	steeringForce /= K_MAX_SPEED;
	// Scale this factor by K_MAX_STEER_FORCE
	steeringForce *= K_MAX_STEER_FORCE;
#else
	steeringForce = SteeringUtils::DoSteeringEvade(target->position, target->speed, position, speed,
		K_MAX_SPEED, K_MAX_STEER_FORCE, maximumLookAhead);
#endif

	// Modify according to mass to get acceleration
	acceleration = steeringForce / mass;

	// Update Speed with acceleration
	speed += acceleration * deltaTime;
}

void Boid::DoSimpleWander(float deltaTime)
{
	float simpleWanderAngle = FloatUtils::RandomBinomial() * wanderMaxChange;
	angle = FloatUtils::Lerp(angle, angle + simpleWanderAngle, 0.3f);
	speed.x = cosf(angle * DEG2RAD) * K_MAX_SPEED;
	speed.y = sinf(angle * DEG2RAD) * K_MAX_SPEED;
}

void Boid::DoFullWander(float deltaTime)
{
#if DEBUG
	// Update wanderAngle
	wanderAngle += FloatUtils::RandomBinomial() * wanderMaxChange;
	float targetAngle = angle + wanderAngle;

	// Calculate the center of the wander circle
	Vector2D speedOrientationVector = Vector2D::Normalize(speed);
	wanderCircleCenter = position + speedOrientationVector * wanderCircleOffset;

	// Calculate and add displacement location (Using the Parametric equation of the circle)
	wanderDisplacementVector.x = wanderCircleRadius * cosf(targetAngle * DEG2RAD);
	wanderDisplacementVector.y = wanderCircleRadius * sinf(targetAngle * DEG2RAD);
	wanderDisplacementVector += wanderCircleCenter;

	// Seek Behavior with target wanderDisplacementVector
	// Calculate desired velocity
	desiredVelocity = wanderDisplacementVector - position;

	// Normalize and Scale by K_MAX_SPEED
	desiredVelocity.Normalize();
	desiredVelocity *= K_MAX_SPEED;

	// Calculate Steering Force
	steeringForce = desiredVelocity - speed;

	// Convert to Force
	// Divide by K_MAX_SPEED to get the speed factor
	steeringForce /= K_MAX_SPEED;
	// Scale this factor by K_MAX_STEER_FORCE
	steeringForce *= K_MAX_STEER_FORCE;
#else
	steeringForce = SteeringUtils::DoSteeringWander(position, speed, K_MAX_SPEED, K_MAX_STEER_FORCE,
		angle, &wanderAngle, wanderMaxChange, wanderCircleOffset, wanderCircleRadius);
#endif

	// Modify according to mass to get acceleration
	acceleration = steeringForce / mass;

	// Update Speed with acceleration
	speed += acceleration * deltaTime;
}

void Boid::DoSimplePathFollowing(float deltaTime)
{
#if DEBUG
	// Find Current segment to target
	Vector2D targetSegment = simplePath->pathArray[currentSegment];

	// Are we near enough targetSegment
	if (Vector2D::Distance(position, targetSegment) < K_SIMPLE_PATH_ARRIVAL_DISTANCE)
	{
		// Update targetSegment next time
		currentSegment += pathDirection;
		if (currentSegment >= simplePath->pathOccupation || currentSegment < 0)
		{
			// Loop
			pathDirection *= -1;
			currentSegment += pathDirection;
		}
	}

	// Seek target segment
	steeringForce = SteeringUtils::DoSteeringSeek(targetSegment, position, speed, K_MAX_SPEED, K_MAX_STEER_FORCE);
#else
	steeringForce = AdvancedSteeringUtils::DoSimplePathFollowing(position, speed, K_MAX_SPEED, K_MAX_STEER_FORCE, 
		simplePath, currentSegment, pathDirection, K_SIMPLE_PATH_ARRIVAL_DISTANCE);
#endif
	// Modify according to mass to get acceleration
	acceleration = steeringForce / mass;

	// Update Speed with acceleration
	speed += acceleration * deltaTime;
}

void Boid::DoPathFollowing(float deltaTime)
{
#if DEBUG
	if (pathFollowingStarted == false)
	{
		float targetParameter = Path_GetParameter(path, position, currentPathTarget);
		currentPathTarget = Path_GetPosition(path, targetParameter);
		pathFollowingStarted = true;
	}
	else if (Vector2D::Distance(position, currentPathTarget) < K_PATH_ARRIVAL_DISTANCE)
	{
		float targetParameter = Path_GetParameter(path, position, currentPathTarget);
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

	steeringForce = SteeringUtils::DoSteeringSeek(currentPathTarget, position, speed, K_MAX_SPEED, K_MAX_STEER_FORCE);
#else
	steeringForce = AdvancedSteeringUtils::DoPathFollowing(position, speed, K_MAX_SPEED, K_MAX_STEER_FORCE, path, pathFollowingStarted,
		currentPathTarget, pathOffset, K_PATH_ARRIVAL_DISTANCE, loopPath);
#endif
	// Modify according to mass to get acceleration
	acceleration = steeringForce / mass;

	// Update Speed with acceleration
	speed += acceleration * deltaTime;
}

void Boid::DoCollisionAvoidance(float deltaTime)
{
#if DEBUG
	float shortestDistance = 999999999.9f;
	Entity* firstTarget = nullptr;

	Vector2D coneBasePoint = { position.x + coneHeight, position.y };
	Vector2D currentConeBase = Vector2DUtils::RotatePoint(position, coneBasePoint, angle);

	collisionDetected = false;

	for (int i = 0; i < numTargets; ++i)
	{
		Entity* currentTarget = targets[i];

		// Is target inside the cone?
		if (Vector2DUtils::IsInsideCone(currentTarget->position, position, currentConeBase, coneHalfAngle))
		{
			float currentDistance = Vector2D::Distance(position, currentTarget->position);
			// Is the nearest targest inside the cone?
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
		return;
	}

	// Flee from collision
	steeringForce = SteeringUtils::DoSteeringFlee(firstTarget->position, position, speed, K_MAX_SPEED, K_MAX_STEER_AVOID_FORCE);
#else
	steeringForce = AdvancedSteeringUtils::DoCollisionAvoidance(position, speed, angle, K_MAX_SPEED, K_MAX_STEER_AVOID_FORCE, (Entity**)targets,
		numTargets, coneHeight, coneHalfAngle, collisionDetected);
#endif
	// Modify according to mass to get acceleration
	acceleration = steeringForce / mass;

	// Update Speed with acceleration
	speed += acceleration * deltaTime;

}

void Boid::AddTargetForCollisionAvoidance(Entity* target)
{
	if (numTargets < MAX_NUMBER_TARGETS)
	{
		targets[numTargets] = target;
		++numTargets;
	}
	else
	{
		SDL_Log("Target Array reached max number of targets.");
	}
}

void Boid::DoObstacleAvoidance(float deltaTime)
{
#if DEBUG
	Vector2D raycastVector = position;
	raycastVector += Vector2D::Normalize(speed) * avoidanceLookahead;

	Vector2D intersectionPoint, normalVector;

	int wallSize = *obstacleArraySize;
	obstacleAvoidanceCollision = false;
	for (int i = 0; i < wallSize; ++i)
	{
		obstacleAvoidanceCollision = Obstacle_IntersectsSegment(&obstacleArray[i], position, raycastVector, intersectionPoint, normalVector);
		if (obstacleAvoidanceCollision == true)
		{
			break;
		}
	}

	if (obstacleAvoidanceCollision == true)
	{
		// Seek where target = normal * avoidanceDistance
		avoidTarget = intersectionPoint;
		avoidTarget += normalVector * avoidanceDistance;

		steeringForce = SteeringUtils::DoSteeringSeek(avoidTarget, position, speed, K_MAX_SPEED, K_MAX_STEER_OBSTACLE_AVOID_FORCE);
	}
	else
	{
		steeringForce = {};
	}
#else
	steeringForce = AdvancedSteeringUtils::DoObstacleAvoidance(position, speed, K_MAX_SPEED,
		K_MAX_STEER_OBSTACLE_AVOID_FORCE, avoidanceLookahead, avoidanceDistance, *obstacleArraySize,
		obstacleArray);
#endif

	// Modify according to mass to get acceleration
	acceleration = steeringForce / mass;

	// Update Speed with acceleration
	speed += acceleration * deltaTime;
}

void Boid::DoFlocking(float deltaTime)
{
#if DEBUG
	alignment = {};
	cohesion = {};
	separation = {};

	int neighbourCount = 0;
	int boidSize = *boidArraySize;

	for (int i = 0; i < boidSize; ++i)
	{
		if (&boidArray[i] == this)
		{
			// Do not count myself
			//SDL_Log("I found me!");
		}
		else
		{
			// Is neighbour?
			if (Vector2D::Distance(position, boidArray[i].position) < K_FLOCK_NEIGHBOURHOOD)
			{
				alignment += boidArray[i].speed;
				cohesion += boidArray[i].position;
				separation += position - boidArray[i].position;
				++neighbourCount;
			}
		}
	}

	if (neighbourCount != 0)
	{
		// Alignment Calculation
		alignment /= neighbourCount;
		// Normalize and scale by K_FLOCK_ALIGNMENT
		alignment.Normalize();
		alignment *= K_FLOCK_ALIGNMENT;

		// Cohesion Calculation
		cohesion /= neighbourCount;
		cohesion -= position;
		// Normalize and scale by K_FLOCK_COHESION
		cohesion.Normalize();
		cohesion *= K_FLOCK_COHESION;

		// Separation Calculation
		separation /= neighbourCount;
		// Normalize and scale by K_FLOCK_SEPARATION
		separation.Normalize();
		separation *= K_FLOCK_SEPARATION;
	}

	// Add all forces together
	flockingForce = alignment + cohesion + separation;

	// Convert to Force
	flockingForce.Normalize();
	flockingForce *= K_MAX_FLOCK_FORCE;
#else
	flockingForce = FlockingUtils::DoFlocking(boidArraySize, boidArray, this, position, K_MAX_FLOCK_FORCE,
		K_FLOCK_NEIGHBOURHOOD, K_FLOCK_ALIGNMENT, K_FLOCK_COHESION, K_FLOCK_SEPARATION);
#endif

	// Modify according to mass to get acceleration
	acceleration = flockingForce / mass;

	// Update Speed with acceleration
	speed += acceleration * deltaTime;

	// Force Limit Speed
	if (speed.Length() > K_MAX_SPEED)
	{
		speed = speed.Normalize() * K_MAX_SPEED;
	}
}

void Boid::DoPerimeterAvoidance(float deltaTime)
{
#if DEBUG
	desiredVelocity = {};

	if (position.x < K_PERIMETER_BORDER)
	{
		desiredVelocity.x = K_MAX_SPEED;
	}
	else if (position.x > FIXED_WIDTH - K_PERIMETER_BORDER)
	{
		desiredVelocity.x = -K_MAX_SPEED;
	}

	if (position.y < K_PERIMETER_BORDER)
	{
		desiredVelocity.y = K_MAX_SPEED;
	}
	else if (position.y > FIXED_HEIGHT - K_PERIMETER_BORDER)
	{
		desiredVelocity.y = -K_MAX_SPEED;
	}

	if (desiredVelocity.Length() > 0.0f)
	{
		// Calculate Steering Force
		steeringForce = desiredVelocity - speed;

		// Convert to Force
		// Divide by K_MAX_SPEED to get the speed factor
		steeringForce /= K_MAX_SPEED;
		// Scale this factor by K_MAX_STEER_FORCE
		steeringForce *= K_MAX_STEER_FORCE;
	}
	else
	{
		steeringForce = {};
	}
#else
	steeringForce = AdvancedSteeringUtils::DoPerimeterAvoidance(position, speed, K_MAX_SPEED, K_MAX_STEER_FORCE,
		FIXED_WIDTH, FIXED_HEIGHT, K_PERIMETER_BORDER);
#endif
	// Modify according to mass to get acceleration
	acceleration = steeringForce / mass;

	// Update Speed with acceleration
	speed += acceleration * deltaTime;
}

void Boid::InitPrioritySteering()
{
	SteeringGroup_Initialize(&group1, 1);
	group1.weightArray[0] = 1.0f;
	group1.calculatedForces[0] = {};

	SteeringGroup_Initialize(&group2, 3);
	group2.weightArray[0] = 1.0f;
	group2.calculatedForces[0] = {};
	group2.weightArray[1] = 1.0f;
	group2.calculatedForces[1] = {};
	group2.weightArray[2] = 1.0f;
	group2.calculatedForces[2] = {};

	groupArray[0] = group1;
	groupArray[1] = group2;
}

void Boid::DeinitPrioritySteering()
{
	SteeringGroup_Free(&group1);
	SteeringGroup_Free(&group2);
}

void Boid::DoPriorityObstacleSteering(float deltaTime)
{
	// Calculate steering
	group1.calculatedForces[0] = AdvancedSteeringUtils::DoObstacleAvoidance(position, speed, K_MAX_SPEED,
		K_MAX_STEER_OBSTACLE_AVOID_FORCE, avoidanceLookahead, avoidanceDistance, *obstacleArraySize,
		obstacleArray);

	// Calculate steering
	group2.calculatedForces[0] = SteeringUtils::DoSteeringWander(position, speed, K_MAX_SPEED, K_MAX_STEER_FORCE,
		angle, &wanderAngle, wanderMaxChange, wanderCircleOffset, wanderCircleRadius);
	steeringForce = CombiningSteering::DoPriorityBlending(2, (SteeringGroup*)groupArray, K_EPSILON_THRESHOLD, usedGroup);

	// Modify according to mass to get acceleration
	acceleration = steeringForce / mass;

	// Update Speed with acceleration
	speed += acceleration * deltaTime;

}

void Boid::DoPriorityCollisionSteering(float deltaTime)
{
	// Calculate steering
	group1.calculatedForces[0] = AdvancedSteeringUtils::DoCollisionAvoidance(position, speed, angle, K_MAX_SPEED, 
		K_MAX_STEER_FORCE, (Entity**)targets, numTargets, coneHeight, coneHalfAngle, collisionDetected);

	// Calculate steering
	group2.calculatedForces[0] = SteeringUtils::DoSteeringWander(position, speed, K_MAX_SPEED, K_MAX_STEER_FORCE,
		angle, &wanderAngle, wanderMaxChange, wanderCircleOffset, wanderCircleRadius);
	steeringForce = CombiningSteering::DoPriorityBlending(2, (SteeringGroup*)groupArray, K_EPSILON_THRESHOLD, usedGroup);

	// Modify according to mass to get acceleration
	acceleration = steeringForce / mass;

	// Update Speed with acceleration
	speed += acceleration * deltaTime;
}

void Boid::DoFlowFieldSteering(float deltaTime)
{
#if DEBUG
	Vector2D desiredVelocity = FlowField_GetForceForPosition(field, position);
	desiredVelocity *= K_MAX_SPEED;

	// Calculate Steering Force
	steeringForce = desiredVelocity - speed;

	// Convert to Force
	// Divide by K_MAX_SPEED to get the speed factor
	steeringForce /= K_MAX_SPEED;
	// Scale this factor by K_MAX_STEER_FLOW_FORCE
	steeringForce *= K_MAX_STEER_FORCE;
#else
	// Calculate steering
	steeringForce = FlowFieldUtils::DoFlowFieldFollowing(position, speed, K_MAX_SPEED,
		K_MAX_STEER_FORCE, field);
#endif
	// Modify according to mass to get acceleration
	acceleration = steeringForce / mass;

	// Update Speed with acceleration
	speed += acceleration * deltaTime;
}

void Boid::DoPriorityFlowFlockingSeek(float deltaTime)
{
	// Calculate steering
	group1.calculatedForces[0] = FlowFieldUtils::DoFlowFieldFollowing(position, speed, K_MAX_SPEED,
		K_MAX_STEER_FORCE, field);

	// Calculate steering
	group2.calculatedForces[0] = FlockingUtils::DoFlocking(boidArraySize, boidArray, this, position, K_MAX_FLOCK_FORCE,
		K_FLOCK_NEIGHBOURHOOD, K_FLOCK_ALIGNMENT, K_FLOCK_COHESION, K_FLOCK_SEPARATION);
	group2.calculatedForces[1] = SteeringUtils::DoSteeringSeek(target->position, position,
		speed, K_MAX_SPEED, K_MAX_STEER_FORCE);
	group2.calculatedForces[2] = AdvancedSteeringUtils::DoPerimeterAvoidance(position, speed, K_MAX_SPEED, K_MAX_STEER_FORCE,
		FIXED_WIDTH, FIXED_HEIGHT, K_PERIMETER_BORDER);

	steeringForce = CombiningSteering::DoPriorityBlending(2, (SteeringGroup*)groupArray, K_EPSILON_THRESHOLD, usedGroup);

	// Modify according to mass to get acceleration
	acceleration = steeringForce / mass;

	// Update Speed with acceleration
	speed += acceleration * deltaTime;
}

// Debug Functions from here till end of file

void Boid::InitDebug(SDL_Renderer* renderer, FC_Font* font)
{
	informationText1.font = font;
	informationText1.renderer = renderer;
	RotatedText_SetText(&informationText1, (char*)"Current Velocity");

	informationText2.font = font;
	informationText2.renderer = renderer;
	RotatedText_SetText(&informationText2, (char*)"Desired Velocity");

	informationText3.font = font;
	informationText3.renderer = renderer;
	RotatedText_SetText(&informationText3, (char*)"Steering Force");

	informationText4.font = font;
	informationText4.renderer = renderer;
	RotatedText_SetText(&informationText4, (char*)"Speed = 0.00");
}

void Boid::InitFlockingDebug(SDL_Renderer* renderer, FC_Font* font)
{
	informationText1.font = font;
	informationText1.renderer = renderer;
	RotatedText_SetText(&informationText1, (char*)"Cohesion");

	informationText2.font = font;
	informationText2.renderer = renderer;
	RotatedText_SetText(&informationText2, (char*)"Alignment");

	informationText3.font = font;
	informationText3.renderer = renderer;
	RotatedText_SetText(&informationText3, (char*)"Separation");

	informationText4.font = font;
	informationText4.renderer = renderer;
	RotatedText_SetText(&informationText4, (char*)"Flocking");
}

void Boid::DeinitDebug()
{
	RotatedText_Free(&informationText1);
	RotatedText_Free(&informationText2);
	RotatedText_Free(&informationText3);
	RotatedText_Free(&informationText4);
}

void Boid::DrawFlockingDebug(SDL_Renderer* renderer)
{
	float drawFactor = 100.0f;

	circleColor(renderer, position.x, position.y, K_FLOCK_NEIGHBOURHOOD, Colors::PUMPKIN);

	SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
	SDL_RenderDrawLine(renderer, position.x, position.y, 
		position.x + alignment.x * K_FLOCK_ALIGNMENT * drawFactor,
		position.y + alignment.y * K_FLOCK_ALIGNMENT * drawFactor);

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderDrawLine(renderer, position.x, position.y, 
		position.x + cohesion.x * K_FLOCK_COHESION * drawFactor,
		position.y + cohesion.y * K_FLOCK_COHESION * drawFactor);

	SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
	SDL_RenderDrawLine(renderer, position.x, position.y,
		position.x + separation.x * K_FLOCK_SEPARATION * drawFactor,
		position.y + separation.y * K_FLOCK_SEPARATION * drawFactor);
}

void Boid::DrawDebug(SDL_Renderer* renderer)
{
	if (currentBehaviour == Behaviour::OBSTACLE_AVOIDANCE)
	{
		if (obstacleAvoidanceCollision == true)
		{
			filledCircleColor(renderer, avoidTarget.x, avoidTarget.y, 10, Colors::TURQUOISE);
		}
	}
	else if (currentBehaviour == Behaviour::COLLISION_AVOIDANCE 
		|| currentBehaviour == Behaviour::PRIORITY_COLLISION)
	{
		static int timerColor = 0;
		int maxTime = 10;
		Uint32 collisionColor = Colors::BELIZE_HOLE;
		if (collisionDetected == true || timerColor > 0)
		{
			collisionColor = Colors::ORANGE;
			if (collisionDetected == true)
			{
				timerColor = maxTime;
			}
		}
		--timerColor;
		Vector2D triangleX1 = { position.x + coneHeight, position.y - tanf(coneHalfAngle * DEG2RAD) * coneHeight };
		Vector2D triangleX2 = { position.x + coneHeight, position.y + tanf(coneHalfAngle * DEG2RAD) * coneHeight };
		Vector2D rotatedTriangle1 = Vector2DUtils::RotatePoint(position, triangleX1, angle);
		Vector2D rotatedTriangle2 = Vector2DUtils::RotatePoint(position, triangleX2, angle);
		filledTrigonColor(renderer, position.x, position.y,
			rotatedTriangle1.x, rotatedTriangle1.y,
			rotatedTriangle2.x, rotatedTriangle2.y,
			collisionColor);
		return;
	}
	else if (currentBehaviour == Behaviour::ARRIVE)
	{
		circleRGBA(renderer, target->position.x, target->position.y, slowingRadius, 255, 255, 0, 255);
		sprintf(textBuffer, "Speed = %.2f", desiredVelocity.Length());
		RotatedText_SetText(&informationText4, textBuffer);
		RotatedText_DrawText(&informationText4, position.x + 10.0f,
			position.y + 10.0f, 0.0f,
			0, 0, 255, 255);
	}
	else if (currentBehaviour == Behaviour::WANDER)
	{
		circleRGBA(renderer, wanderCircleCenter.x, wanderCircleCenter.y, wanderCircleRadius, 255, 255, 0, 255);
		filledCircleRGBA(renderer, wanderDisplacementVector.x, wanderDisplacementVector.y, 10, 128, 0, 128, 255);

		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderDrawLine(renderer, position.x, position.y, wanderDisplacementVector.x, wanderDisplacementVector.y);
	}
	else if (currentBehaviour != Behaviour::PRIORITY_OBSTACLE)
	{
		int currentTextAngle;

		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderDrawLine(renderer, position.x, position.y, position.x + speed.x, position.y + speed.y);

		if (isSteering == true)
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
			SDL_RenderDrawLine(renderer, position.x, position.y, position.x + desiredVelocity.x, position.y + desiredVelocity.y);

			SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
			SDL_RenderDrawLine(renderer, position.x + speed.x, position.y + speed.y, position.x + desiredVelocity.x, position.y + desiredVelocity.y);

			currentTextAngle = atan2f(desiredVelocity.y, desiredVelocity.x) * RAD2DEG;
			if (currentTextAngle < -90 || (currentTextAngle > 90 && currentTextAngle < 180))
			{
				currentTextAngle = currentTextAngle - 180;
			}
			RotatedText_DrawText(&informationText2,
				(2 * position.x + (desiredVelocity.x)) / 2,
				(2 * position.y + desiredVelocity.y) / 2,
				currentTextAngle,
				255, 255, 0, 255);

			currentTextAngle = atan2f((desiredVelocity.y - speed.y), (desiredVelocity.x - speed.x)) * RAD2DEG;
			if (currentTextAngle < -90 || (currentTextAngle > 90 && currentTextAngle < 180))
			{
				currentTextAngle = currentTextAngle - 180;
			}
			RotatedText_DrawText(&informationText3,
				(position.x + (speed.x) + position.x + (desiredVelocity.x)) / 2,
				(position.y + speed.y + position.y + desiredVelocity.y) / 2,
				currentTextAngle,
				255, 0, 255, 255);

			if (currentBehaviour == Behaviour::PURSUE || currentBehaviour == Behaviour::EVADE)
			{
				filledCircleRGBA(renderer, futurePosition.x, futurePosition.y, 20, 255, 255, 0, 255);
			}
		}

		currentTextAngle = angle;
		if (currentTextAngle < -90 || (currentTextAngle > 90 && currentTextAngle < 180))
		{
			currentTextAngle = currentTextAngle - 180;
		}
		RotatedText_DrawText(&informationText1,
			(2 * position.x + (speed.x)) / 2,
			(2 * position.y + speed.y) / 2,
			currentTextAngle,
			255, 0, 0, 255);
	}
}

void Boid::Trace(Texture* entityTexture, SDL_Renderer* renderer, int step)
{
	// Keep current alpha and change it to step by step
	Uint8 alphaOld = Texture_GetAlpha(entityTexture);
	Uint8 alphaInit = 80;
	Uint8 alphaDelta = alphaInit / step;
	Uint8 alphaToUse = alphaInit - alphaDelta;
	
	// In/Out params to modify
	Vector2D stepPosition = position;
	Vector2D stepSpeed = speed;
	int stepAngle = 0;

	for (int i = 0; i < step; ++i)
	{
		// Simulate Step
		SimulateUpdate(1 / 60.0f, stepPosition, stepSpeed, stepAngle, target->position);

		// Update Alpha
		Texture_SetAlpha(entityTexture, alphaToUse);
		alphaToUse -= alphaDelta;

		// Render Simulation
		Texture_Render(entityTexture, renderer,
			(int)stepPosition.x - (width * scaleX) / 2,
			(int)stepPosition.y - (height * scaleY) / 2,
			nullptr, scaleX, scaleY, stepAngle);
	}
	
	// Restore Alpha
	Texture_SetAlpha(entityTexture, alphaOld);
}

void Boid::SimulateUpdate(float deltaTime, Vector2D& position, Vector2D& speed, int& angle, Vector2D targetPosition)
{
	Vector2D steeringForce = SteeringUtils::DoSteeringSeek(targetPosition, position, speed, K_MAX_SPEED, K_MAX_STEER_FORCE);

	// Modify according to mass to get acceleration
	Vector2D acceleration = steeringForce / mass;

	// Update Speed with acceleration
	speed += acceleration * deltaTime;

	// Move entity

	// Add speed to speed counter and get number of pixels to move this frame (integer)
	traceSpeedCounter += (speed * deltaTime);
	Vector2D realSpeed = {};
	realSpeed.x = traceSpeedCounter.x > 0.0f ? floor(traceSpeedCounter.x) : ceil(traceSpeedCounter.x);
	realSpeed.y = traceSpeedCounter.y > 0.0f ? floor(traceSpeedCounter.y) : ceil(traceSpeedCounter.y);

	// Remainder for next frame
	traceSpeedCounter -= realSpeed;

	// Move position
	position += realSpeed;

	// Update Orientation if speed is higher than a safety threshold
	if (speed.Length() > 20.0f)
	{
		angle = FloatUtils::CalculateOrientation(speed);
	}
}

