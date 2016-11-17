/* ========================================================================
   File: FlowFieldState.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Common/GameFramework.h"
#include "Boid.h"
#include "Player.h"

#define BOID_POOL_SIZE 30
#define INITIAL_NUMBER_BOIDS 15
#define FLOW_OBSTACLE_POOL_SIZE 10

class FlowFieldState : public BaseState
{
public:
	virtual void Init() override;
	virtual void Deinit() override;
	virtual StateCode HandleInput() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;
	void CreateBoid(int x, int y, Behaviour behaviour);
	void ChangeBoidsBehavior(Behaviour newBehavior);
	void CreateObstacle(Vector2D position);

	// State Data

	// Graphics
	Texture boidTexture;
	Texture playerTexture;

	// Entities
	Boid boidPool[BOID_POOL_SIZE];
	int boidPoolOccupation = 0;
	Player player;

	// Flow Field
	FlowField field;

	// Flow Field Obstacles
	FlowObstacle obstaclePool[FLOW_OBSTACLE_POOL_SIZE];
	int obstaclePoolOccupation = 0;

	// Fonts and Debug
	FC_Font* fontVerySmall;
	FC_Font* fontSmall;
	FC_Font* fontBig;
	char* title;
	char* theoricUpdateInformation;
	char* theroicAlgorithmInformation;
	char mouseInformation[256];
	char smoothInformation[256];
	bool showDebug = false;

	// Pause Params
	bool isPaused = false;
};

