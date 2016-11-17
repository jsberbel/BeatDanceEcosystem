/* ========================================================================
   File: BaseState.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include <string>
#include "..\Constants.h"
#include "Utilities.h"
#include "Display.h"

class BaseState {
public:
	// Internals
	StateCode name = MENU;
	bool paused = false;
	
	// Handlers
	Display* display = nullptr;

	// Resources Loading helpers
	std::string resourcesFolderPath;
	const char* resourcesFilePath = nullptr;

	// Virtual Functions
	virtual void Init() = 0;
	virtual void Deinit() = 0;
	virtual StateCode HandleInput() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render() = 0;

	void SetResourcesFilePath(const char* ResourcesPathAndFile);
};



