/* ========================================================================
   File: App.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "App.h"

bool InitializeApp()
{
	if (SDL_Init(0) < 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "SDL_Init: Couldn't start SDL\n");
		return false;
	}
	else
	{
		// Initializing Subsystems
		int flags = (SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER);
		if (SDL_InitSubSystem(flags) < 0)
		{
			SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "SDL_Init: Couldn't start Subsystems.\n");
			SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "SDL_GetError = %s.\n", SDL_GetError());
			return false;
		}
		
		//// Initializing IMG - Image support
		int imgFlags = IMG_INIT_PNG;
		if (!IMG_Init(imgFlags) & imgFlags)
		{
			SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "IMG_Init: Couldn't start Image support.\n");
			SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "IMG_GetError = %s\n", IMG_GetError());
			return false;
		}
		
		// Initialize TTF support
		if (TTF_Init() == -1)
		{
			SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "SDL_ttf could not be initialized. TTF_Error: %s\n", TTF_GetError());
			return false;
		}
		
		// Initialize SDL_mixer.
		if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
		{
			SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "SDL_mixer could not be initialized. Mix_Error: %s\n", Mix_GetError());
			return false;
		}
	}
	
	return true;
}

void CloseApp()
{
	// Cleanup
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

