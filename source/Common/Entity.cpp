/* ========================================================================
   File: Entity.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "Entity.h"
#include "Utilities.h"

void Entity::Update(float deltaTime)
{
	// Move entity

	// Add speed to speed counter and get number of pixels to move this frame (integer)
	speedCounter += (speed * deltaTime);
	realSpeed.x = speedCounter.x > 0.0f ? floor(speedCounter.x) : ceil(speedCounter.x);
	realSpeed.y = speedCounter.y > 0.0f ? floor(speedCounter.y) : ceil(speedCounter.y);

	// Remainder for next frame
	speedCounter -= realSpeed;

	// Move position
	position += realSpeed;

	// Update Orientation if speed is higher than a safety threshold
	if (speed.Length() > 20.0f)
	{
		if (smoothAngleChange == true)
		{
			float attemptedAngle = FloatUtils::CalculateOrientation(speed);
			float angleDelta = attemptedAngle - angle;
			if (angleDelta > 180.0f)
			{
				angle += 360.0f;
			}
			else if (angleDelta < -180.0f)
			{
				angle -= 360.0f;
			}
			angle = FloatUtils::Lerp(angle, attemptedAngle, 0.1f);
		}
		else
		{
			angle = FloatUtils::CalculateOrientation(speed);
		}
	}
}

void Entity::Render(Texture* entityTexture, SDL_Renderer* renderer)
{
	Texture_Render(entityTexture, renderer,
		(int)position.x - (width * scaleX) / 2,
		(int)position.y - (height * scaleY) / 2,
		nullptr, scaleX, scaleY, angle);
}







