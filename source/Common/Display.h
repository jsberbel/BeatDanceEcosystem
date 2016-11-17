/* ========================================================================
   File: Display.h
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

struct Display
{
    int width = 0;
    int height = 0;
    
    const char *title = nullptr;
    SDL_Color backgroundColor = {0, 0, 0, 255};
    
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    
    bool isFullscreen = false;
};

void Display_Initialize(Display* display,
                                            unsigned int width,
                                            unsigned int height,
                                            const char* title);
void Display_Free(Display* display);

void Display_Fill(Display* display, SDL_Color color);
void Display_Clear(Display* display);
void Display_RefreshWindow(Display* display);


