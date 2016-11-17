/* ========================================================================
   File: Boid.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Common/GameFramework.h"
#include "SimplePath.h"
#include "Path.h"
#include "Obstacle.h"
#include "CombiningSteeringUtils.h"
#include "FlowFieldUtils.h"

enum Behaviour
{
	NONE,
	SEEK_KINEMATIC,
	SEEK_STEERING,
	FLEE_KINEMATIC,
	FLEE_STEERING,
	ARRIVE,
	PURSUE,
	EVADE,
	WANDER,
	SIMPLE_WANDER,
	SIMPLE_PATH_FOLLOWING,
	PATH_FOLLOWING,
	COLLISION_AVOIDANCE,
	OBSTACLE_AVOIDANCE,
	FLOCKING,
	FLOCKING_SEEK,
	PRIORITY_OBSTACLE,
	PRIORITY_COLLISION,
	FLOW_FLOCK_SEEK_PRIORITY
};

#define MAX_NUMBER_TARGETS 10

// Make it global for easy debug manipulation
static float K_FLOCK_COHESION = 0.4f;
static float K_FLOCK_ALIGNMENT = 0.6f;
static float K_FLOCK_SEPARATION = 1.7f;

struct Boid : public Entity
{
	// Constants
	float K_MAX_SPEED = 200.0f;
	//float K_MAX_STEER_FORCE = 6.0f * 60.0f; // For Seek + Flocking
	float K_MAX_STEER_FORCE = 15.0f * 60.0f; // Normal

											 // State fields
	Behaviour currentBehaviour = SEEK_KINEMATIC;
	bool isSteering = false;

	// Steering fields
	Entity* target = nullptr;
	Vector2D desiredVelocity = {};
	Vector2D steeringForce = {};
	Vector2D acceleration = {};
	float mass = 1.0f;

	// Arrive fields
	int slowingRadius = 100;

	// Pursue fields
	Vector2D futurePosition = {};
	float maximumLookAhead = 0.5f;
	float timeLookAhead = 0.7f;

	// Wander fields
	int wanderMaxChange = 50;
	int wanderCircleOffset = 80;
	int wanderCircleRadius = 70;
	float wanderAngle = 0.0f;
	Vector2D wanderCircleCenter = {};
	Vector2D wanderDisplacementVector = {};

	// Simple Path Following
	SimplePath* simplePath = nullptr;
	int currentSegment = 0;
	int pathDirection = 1;
	float K_SIMPLE_PATH_ARRIVAL_DISTANCE = 20.0f;

	// Path Following
	Path* path = nullptr;
	float pathOffset = 0.08f;
	Vector2D currentPathTarget;
	bool loopPath = true;
	bool pathFollowingStarted = false;
	float K_PATH_ARRIVAL_DISTANCE = 10.0f;

	// Flocking
	Boid* boidArray;
	int* boidArraySize;
	Vector2D alignment;		// Debug purposes
	Vector2D cohesion;		// Debug purposes
	Vector2D separation;	// Debug purposes
	Vector2D flockingForce; // Debug purposes
	float K_MAX_FLOCK_FORCE = 20.0f * 60.0f;
	float K_FLOCK_NEIGHBOURHOOD = 60.0f;

	// Perimeter avoidance
	float K_PERIMETER_BORDER = 20.0f;

	// Obstacle Avoidance
	Obstacle* obstacleArray;
	int* obstacleArraySize;
	float avoidanceLookahead = 100.0f;
	float avoidanceDistance = 30.0f;
	float K_MAX_STEER_OBSTACLE_AVOID_FORCE = 60.0f * 60.0f;
	Vector2D avoidTarget = {}; // Debug purposes
	bool obstacleAvoidanceCollision = false; // Debug purposes

											 // Collision Avoidance
	float K_MAX_STEER_AVOID_FORCE = 13.0f * 60.0f;
	Entity* targets[MAX_NUMBER_TARGETS];
	int numTargets = 0;
	float coneHeight = 80.0f;
	float coneHalfAngle = 30.0f;
	void AddTargetForCollisionAvoidance(Entity* target);
	bool collisionDetected = false; // Debug purposes

									// Priority Steering
	SteeringGroup groupArray[2];
	SteeringGroup group1;
	SteeringGroup group2;
	float K_EPSILON_THRESHOLD = 11.0f * 60.0f;
	int usedGroup = 0;

	// Flow Field Steering
	FlowField* field;

	// Methods
	virtual void Update(float deltaTime) override;
	void SetBehaviour(Behaviour newBehaviour);
	void DoKinematicSeek();
	void DoKinematicFlee();
	void DoSteeringSeek(float deltaTime);
	void DoSteeringFlee(float deltaTime);
	void DoArrive(float deltaTime);
	void DoPursue(float deltaTime);
	void DoEvade(float deltaTime);
	void DoSimpleWander(float deltaTime);
	void DoFullWander(float deltaTime);
	void DoSimplePathFollowing(float deltaTime);
	void DoPathFollowing(float deltaTime);
	void DoCollisionAvoidance(float deltaTime);
	void DoObstacleAvoidance(float deltaTime);
	void DoFlocking(float deltaTime);
	void DoPerimeterAvoidance(float deltaTime);
	void InitPrioritySteering();
	void DoPriorityObstacleSteering(float deltaTime);
	void DoPriorityCollisionSteering(float deltaTime);
	void DeinitPrioritySteering();
	void DoFlowFieldSteering(float deltaTime);
	void DoPriorityFlowFlockingSeek(float deltaTime);

	// Debug Methods
	void InitDebug(SDL_Renderer* renderer, FC_Font* font);
	void InitFlockingDebug(SDL_Renderer* renderer, FC_Font* font);
	void DrawDebug(SDL_Renderer* renderer);
	void DrawFlockingDebug(SDL_Renderer* renderer);
	void DeinitDebug();

	// Trace methods and fields
	void Trace(Texture* entityTexture, SDL_Renderer* renderer, int step);
	void SimulateUpdate(float deltaTime, Vector2D& position, Vector2D& speed, int& angle, Vector2D targetPosition);
	Vector2D traceSpeedCounter = {};

	// Debug Information
	RotatedText informationText1;
	RotatedText informationText2;
	RotatedText informationText3;
	RotatedText informationText4;
	char textBuffer[100];
};

