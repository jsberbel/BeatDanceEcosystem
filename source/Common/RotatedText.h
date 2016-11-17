/* ========================================================================
   File: RotatedText.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "SDL_FontCache/SDL_FontCache.h"
#include "Texture.h"

struct RotatedText
{
    Texture targetedTexture;
    FC_Font* font = nullptr;
    SDL_Renderer* renderer = nullptr;
    char* text = nullptr;
};

void RotatedText_Free(RotatedText* rotatedText);

void RotatedText_SetText(RotatedText* rotatedText, char* text);

void RotatedText_DrawText(RotatedText* rotatedText,
                                         int x, int y, float angle,
                                         Uint8 r, Uint8 g, Uint8 b, Uint8 a);

