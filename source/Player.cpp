/* ========================================================================
   File: Player.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "Player.h"

void Player::HandleInput()
{
	// Keyboard movement
	Uint8 keyUp = InputSingleton::Instance()->GetKey(SDL_SCANCODE_UP);
	Uint8 keyDown = InputSingleton::Instance()->GetKey(SDL_SCANCODE_DOWN);
	Uint8 keyLeft = InputSingleton::Instance()->GetKey(SDL_SCANCODE_LEFT);
	Uint8 keyRight = InputSingleton::Instance()->GetKey(SDL_SCANCODE_RIGHT);
	
	// Gamepad movement
	keyUp |= InputSingleton::Instance()->gamepadButtonsState.dPadUp.isDown;
	keyDown |= InputSingleton::Instance()->gamepadButtonsState.dPadDown.isDown;
	keyLeft |= InputSingleton::Instance()->gamepadButtonsState.dPadLeft.isDown;
	keyRight |= InputSingleton::Instance()->gamepadButtonsState.dPadRight.isDown;

	moveHorizontal = keyRight - keyLeft;
	moveVertical = keyDown - keyUp;
}

void Player::Update(float deltaTime)
{
	float fric = Player::K_RUN_FRIC * deltaTime;
	float accel = Player::K_RUN_ACCEL * deltaTime;
	
	if (moveHorizontal > 0)
	{
		// Moving right
		// First add friction if currently going left
		if (speed.x < 0)
		{
			speed.x = FloatUtils::Approach(speed.x, 0, fric);
		}
		speed.x = FloatUtils::Approach(speed.x, K_MAX_SPEED, accel);
	}
	else if (moveHorizontal < 0)
	{
		// Moving left
		// First add friction if currently going right
		if (speed.x > 0)
		{
			speed.x = FloatUtils::Approach(speed.x, 0, fric);
		}
		speed.x = FloatUtils::Approach(speed.x, -K_MAX_SPEED, accel);
	}
	else
	{
		// Stopping by friction
		speed.x = FloatUtils::Approach(speed.x, 0, fric);
	}
	
	if (moveVertical > 0)
	{
		// Moving down
		// First add friction if currently going up
		if (speed.y < 0)
		{
			speed.y = FloatUtils::Approach(speed.y, 0, fric);
		}
		speed.y = FloatUtils::Approach(speed.y, K_MAX_SPEED, accel);
	}
	else if (moveVertical < 0)
	{
		// Moving up
		// First add friction if currently going down
		if (speed.y > 0)
		{
			speed.y = FloatUtils::Approach(speed.y, 0, fric);
		}
		speed.y = FloatUtils::Approach(speed.y, -K_MAX_SPEED, accel);
	}
	else
	{
		// Stopping by friction
		speed.y = FloatUtils::Approach(speed.y, 0, fric);
	}

	// Clamp speed
	if (speed.Length() > K_MAX_SPEED)
	{
		speed.Normalize();
		speed *= K_MAX_SPEED;
	}

	// Keep angle for later
	float oldAngle = angle;

	// Call base Update
	Entity::Update(deltaTime);

	// NOTE(dcollado): Check this!
	// Override Orientation: It follows speed direction if speed is higher than threshold (magic number!)
	if (speed.Length() > 60.0f)
	{
		angle = FloatUtils::CalculateOrientation(speed);
	}
	else
	{
		angle = oldAngle;
	}

	// Wrap player
	Utils::DoWrap(position);
}




