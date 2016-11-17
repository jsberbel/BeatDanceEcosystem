/* ========================================================================
   File: EcosystemState.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Common/GameFramework.h"
#include "Boid.h"
#include "Player.h"
#include "BeatDetector.h"

#define MAX_SONGS 25
#define SAMPLE_SIZE 1024
#define SPAWN_TIME 10000
#define MAX_BOIDS 50
#define INITIAL_BOIDS 5
#define FLOW_OBSTACLE_POOL_SIZE 10

class EcosystemState : public BaseState
{
public:
	virtual void Init() override;
	virtual void Deinit() override;
	virtual StateCode HandleInput() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;
	bool CheckCollision() const;
	void PlaySong(int num, int mult);
	void AddSong(char* path);
	void CreateBoid(int x, int y, Behaviour behaviour);
	void ChangeBoidsBehavior(Behaviour newBehavior);

	// State Data
	Behaviour curBehavior;
	Behaviour prevBehavior;
	bool gameOver = false;
	int gameScore = 0;

	// Graphics
	Texture boidTexture;
	Texture playerTexture;
	Texture groundTexture;
	Texture barTexture;

	// Entities
	Boid boidPool[MAX_BOIDS];
	int boidPoolOccupation = 0;
	Player player;

	// Flow Field
	FlowField field;

	// Fonts and Debug
	FC_Font* fontVerySmall;
	FC_Font* fontSmall;
	FC_Font* fontBig;
	char* title;
	char* theoricUpdateInformation;
	char* theroicAlgorithmInformation;
	char mouseInformation[256];
	char smoothInformation[256];
	char scoreInformation[256];
	char enemiesInformation[256];
	bool showDebug = false;

	// Pause Params
	bool isPaused = false;

	// Beat Detector
	BeatDetector* beatDetector = nullptr;
	TimeStamp* lastBeat = nullptr;
	float danceDeltaTime = 0.0f;
	float danceMult = 1.0f;
	char songNames[MAX_SONGS][256];
	int songOccupation = 0;
	int reverseCounter = 0;
	clock_t timeCounter = 0.0f;
};