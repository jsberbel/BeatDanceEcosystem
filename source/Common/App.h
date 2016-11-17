/* ========================================================================
   File: App.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#if __APPLE__
#include <SDL2/SDL.h>            // SDL2
#include <SDL2_image/SDL_image.h>       // SDL2_Image
#include <SDL2_ttf/SDL_ttf.h>           // SDL2_ttf
#include <SDL2_mixer/SDL_mixer.h>       // SDL2_mixer
#else
#include <SDL.h>                   // SDL2
#include <SDL_image.h>       // SDL2_Image
#include <SDL_ttf.h>           // SDL2_ttf
#include <SDL_mixer.h>       // SDL2_mixer
#endif

bool InitializeApp();
void CloseApp();

