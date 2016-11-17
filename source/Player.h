/* ========================================================================
   File: Player.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Common/GameFramework.h"

struct Player : public Entity
{
	// Constants
	float K_RUN_ACCEL = 25.0f * 60.0f;
	float K_RUN_FRIC = 10.5f * 60.0f;
	float K_MAX_SPEED = 450.0f;

	// Status flags
	bool isRunning = false;

	// Input flags
	int moveHorizontal = 0;
	int moveVertical = 0;

	virtual void HandleInput();
	virtual void Update(float deltaTime) override;
};