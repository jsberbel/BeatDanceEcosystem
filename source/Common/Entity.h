/* ========================================================================
   File: Entity.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Vector2D.h"
#include "Texture.h"

struct Entity
{
	// Linear Movement
	Vector2D position = {};
	Vector2D speed = {};
	Vector2D speedCounter = {};
	Vector2D realSpeed = {};

	// Orientation
	float angle = 0.0f;
	bool smoothAngleChange = false;

	// Size
	int width = 0;
	int height = 0;
	int scaleX = 1;
	int scaleY = 1;

	// Methods
	virtual void Update(float deltaTime);
	virtual void Render(Texture* entityTexture, SDL_Renderer* renderer);
};






