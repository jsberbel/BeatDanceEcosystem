/* ========================================================================
   File: StateManager.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Display.h"
#include "BaseState.h"

struct StateManager
{
	Display* display = nullptr;
	BaseState* currentState = nullptr;

	typedef bool(*SwitcherCallbackFunctionType)(StateManager* manager, StateCode currentStateCode);
	SwitcherCallbackFunctionType switcherCallback;
};

void StateManager_Free(StateManager* manager);
void StateManager_RunCurrentState(StateManager* manager);
void StateManager_ChangeState(StateManager* manager, BaseState* gameState);


