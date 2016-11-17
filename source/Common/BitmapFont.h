/* ========================================================================
   File: BitmapFont.h
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

#include <string>
#include "Texture.h"

struct BitmapFont
{
    // The font texture
    Texture* bitmap = nullptr;
    // The individual characters in the surface
    SDL_Rect chars[256] = {};
    // Spacing variables
    int newLine = 0;
    int space = 0;
};

void BitmapFont_Free(BitmapFont* bitmapFont);
// Generates the font
bool BitmapFont_BuildFont(BitmapFont* bitmapFont, Texture* bitmap);
// Shows the text
void BitmapFont_RenderText(BitmapFont* bitmapFont, SDL_Renderer* renderer, int x, int y, std::string text);

