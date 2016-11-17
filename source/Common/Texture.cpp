/* ========================================================================
   File: Texture.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "Texture.h"

#if __APPLE__
#include <SDL2_image/SDL_image.h>
#else
#include <SDL_image.h>
#endif

void Texture_Free(Texture* texture)
{
	if (texture->texture != nullptr)
	{
		SDL_DestroyTexture(texture->texture);
		texture->texture = nullptr;
		texture->width = 0;
		texture->height = 0;
		texture->pitch = 0;
	}
}

void Texture_Render(Texture* texture, SDL_Renderer* renderer, int x, int y, SDL_Rect* clip,
					 double scaleX, double scaleY, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	SDL_Rect renderQuad  = {x, y, texture->width, texture->height};
	
	if (clip != nullptr)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	
	renderQuad.w *= (int)scaleX;
	renderQuad.h *= (int)scaleY;
	
	SDL_RenderCopyEx(renderer, texture->texture, clip, &renderQuad, angle, center, flip);
}

bool Texture_LoadFromFile(Texture* texture, SDL_Renderer* renderer, const char* path, bool doColorManipulation)
{
	Texture_Free(texture);

	bool success = true;
	SDL_Surface* loadedSurface = IMG_Load(path);
	if (loadedSurface == nullptr)
	{
		SDL_Log("Unable to load image %s.\nIMG_Error: %s\n", path, IMG_GetError());
		success = false;
	}
	else
	{
		texture->texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (texture->texture == nullptr)
		{
			SDL_Log("Unable to create texture from %s.\nSDL_Error: %s\n", path, SDL_GetError());
			success = false;
		}
		else
		{
			texture->width = loadedSurface->w;
			texture->height = loadedSurface->h;
		}
	}
	SDL_FreeSurface(loadedSurface);

	return texture->texture != nullptr;
}

bool Texture_LockTexture(Texture* texture)
{
	bool success = true;

	if (texture->pixels != nullptr)
	{
		SDL_Log("Texture already locked!\n");
		success = false;
	}
	else
	{
		if (SDL_LockTexture(texture->texture, NULL, &texture->pixels, &texture->pitch) != 0)
		{
			SDL_Log("Unable to lock texture! %s\n", SDL_GetError());
			success = false;
		}
	}

	return success;
}

bool Texture_UnlockTexture(Texture* texture)
{
	bool success = true;

	if (texture->pixels == nullptr)
	{
		SDL_Log("Texture is not locked!\n");
		success = false;
	}
	else
	{
		SDL_UnlockTexture(texture->texture);
		texture->pixels = nullptr;
		texture->pitch = 0;
	}
	return success;
}

int Texture_GetAlpha(Texture* texture)
{
	Uint8 currentAlpha;
	int result = SDL_GetTextureAlphaMod(texture->texture, &currentAlpha);
	return (result == 0 ? currentAlpha : -1);
}

bool Texture_SetAlpha(Texture* texture, Uint8 alpha)
{
	int result = SDL_SetTextureAlphaMod(texture->texture, alpha);
	return (result == 0);
}

Uint32 Texture_GetPixel32(Texture* texture, unsigned int x, unsigned int y)
{
	//Convert the pixels to 32 bits
	Uint32* pixels = (Uint32*)texture->pixels;

	return pixels[(y * texture->pitch / 4) + x];

	return 0;
}

bool Texture_CreateBlank(Texture* texture, SDL_Renderer* renderer, int width, int height, SDL_TextureAccess access)
{
	//Create uninitialized texture
	texture->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, access, width, height);
	if (texture->texture == nullptr)
	{
		SDL_Log("Unable to create blank texture! SDL Error: %s\n", SDL_GetError());
	}
	else
	{
		texture->width = width;
		texture->height = height;
	}

	return texture->texture != nullptr;
}

void Texture_SetAsRenderTarget(Texture* texture, SDL_Renderer* renderer)
{
	//Make self render target
	SDL_SetRenderTarget(renderer, texture->texture);
}

void Texture_SetDefaultTarget(SDL_Renderer* renderer)
{
	//Make self render target
	SDL_SetRenderTarget(renderer, NULL);
}

