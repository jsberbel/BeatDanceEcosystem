/* ========================================================================
   File: Texture.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#if __APPLE__
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif

struct Texture
{
	// The hardware texture
	SDL_Texture* texture = nullptr;
	void* pixels = nullptr;
	int pitch = 0;
	// Image dimensions
	int width = 0;
	int height = 0;
};


void Texture_Free(Texture* texture);
void Texture_Render(Texture* texture, SDL_Renderer* renderer, int x, int y, SDL_Rect* clip = nullptr,
					 double scaleX = 1.0, double scaleY = 1.0, double angle = 0.0, SDL_Point* center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE);
bool Texture_LoadFromFile(Texture* texture, SDL_Renderer* renderer,
	const char* path, bool doColorManipulation = false);

bool Texture_LockTexture(Texture* texture);
bool Texture_UnlockTexture(Texture* texture);

int Texture_GetAlpha(Texture* texture);
bool Texture_SetAlpha(Texture* texture, Uint8 alpha);

Uint32 Texture_GetPixel32(Texture* texture, unsigned int x, unsigned int y);
bool Texture_CreateBlank(Texture* texture, SDL_Renderer* renderer,
											int width, int height, SDL_TextureAccess access);
void Texture_SetAsRenderTarget(Texture* texture, SDL_Renderer* renderer);
void Texture_SetDefaultTarget(SDL_Renderer* renderer);

