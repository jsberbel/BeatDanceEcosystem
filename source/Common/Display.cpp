/* ========================================================================
   File: Display.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "Display.h"

#include "../Constants.h"

void Display_Initialize(Display* handler, unsigned int width, unsigned int height, const char* title = "")
{
	handler->title = title;
	handler->backgroundColor = {0, 184, 167, 255};
	handler->isFullscreen = false;

	float zoomLevel = 1.0f;

	// NOTE(dcollado): Should I adjust zoom level for Retina Displays?
	//SDL_GetDisplayDPI(0, &zoomLevel, nullptr, nullptr);
	//zoomLevel /= 96.0f; // Magic Number for standard DPI
	//zoomLevel = ceilf(zoomLevel);
	//SDL_Log("Zoom Level = %f.\n", zoomLevel);
	
	handler->width = (int)(width * zoomLevel);
	handler->height = (int)(height * zoomLevel);

	handler->window = SDL_CreateWindow(handler->title,
									   SDL_WINDOWPOS_CENTERED,
									   SDL_WINDOWPOS_CENTERED,
									   handler->width,
									   handler->height,
									   SDL_WINDOW_SHOWN);

	if (handler->window == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Window could not be created.\n");
		SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "SDL_GetError = %s.\n", SDL_GetError());
	}
	else
	{
		handler->renderer = SDL_CreateRenderer(handler->window, -1,
											   SDL_RENDERER_ACCELERATED
											   | SDL_RENDERER_PRESENTVSYNC);
		if (handler->renderer == NULL)
		{
			SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Renderer could not be created.\n");
			SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "SDL_GetError = %s.\n", SDL_GetError());
		}

		SDL_RenderSetLogicalSize(handler->renderer, FIXED_WIDTH, FIXED_HEIGHT);
	}
}

void Display_Free(Display* display)
{
	if (display->renderer)
	{
		SDL_DestroyRenderer(display->renderer);
		display->renderer = nullptr;
	}
	
	if (display->window)
	{
		SDL_DestroyWindow(display->window);
		display->window = nullptr;
	}
}

void Display_Fill(Display* handler, SDL_Color color)
{
	SDL_SetRenderDrawColor(handler->renderer,
		color.r,
		color.g,
		color.b,
		color.a);
	SDL_RenderClear(handler->renderer);
}

void Display_Clear(Display* handler)
{
	Display_Fill(handler, handler->backgroundColor);
}

void Display_RefreshWindow(Display* handler)
{
	SDL_RenderPresent(handler->renderer);
}


