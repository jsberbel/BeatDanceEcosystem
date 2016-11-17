/* ========================================================================
   File: MenuState.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Common/GameFramework.h"

class MenuState : public BaseState
{
public:
	virtual void Init() override;
	virtual void Deinit() override;
	virtual StateCode HandleInput() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;

	// Text
	FC_Font* fontSmall;
	FC_Font* fontBig;

	// Vars to draw Start Message
	float K_DRAW_PROMPT_THRESHOLD = 0.5f;
	float drawPromptCounter = false;
	bool drawPrompt = 0.0f;

	Uint32 curColor = Colors::DISCO;
};