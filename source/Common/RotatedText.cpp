/* ========================================================================
   File: RotatedText.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "RotatedText.h"

void RotatedText_Free(RotatedText* rotatedText)
{
    rotatedText->font = nullptr;
    rotatedText->text = nullptr;
    rotatedText->renderer = nullptr;
    Texture_Free(&rotatedText->targetedTexture);
}

void RotatedText_SetText(RotatedText* rotatedText, char* text)
{
    Uint16 width = FC_GetWidth(rotatedText->font, text);
    Uint16 height = FC_GetHeight(rotatedText->font, text);
    
    Texture_CreateBlank(&rotatedText->targetedTexture, rotatedText->renderer, width, height, SDL_TEXTUREACCESS_TARGET);
    SDL_SetTextureBlendMode(rotatedText->targetedTexture.texture, SDL_BLENDMODE_BLEND);
    rotatedText->text = text;
}

void RotatedText_DrawText(RotatedText* rotatedText,
                                         int x, int y, float angle,
                                         Uint8 r = 0x00, Uint8 g = 0x00, Uint8 b = 0x00, Uint8 a = 0xFF)
{
    //Set self as render target
    Texture_SetAsRenderTarget(&rotatedText->targetedTexture, rotatedText->renderer);
    
    //Clear screen
    SDL_SetRenderDrawColor(rotatedText->renderer, 0xFF, 0xFF, 0xFF, 0x00);
    SDL_RenderClear(rotatedText->renderer);
    
    FC_Effect effect = {
            FC_ALIGN_LEFT,
            {1.0f, 1.0f},
            {r, g, b, a}
    };
    
    FC_DrawEffect(rotatedText->font, rotatedText->renderer, 0, 0, effect, rotatedText->text);
    
    Texture_SetDefaultTarget(rotatedText->renderer);
    
    SDL_Point pointOfRotation = {FC_GetWidth(rotatedText->font, rotatedText->text)/2,
                                    FC_GetHeight(rotatedText->font, rotatedText->text)/2};
    Texture_Render(&rotatedText->targetedTexture, rotatedText->renderer,
                    x - FC_GetWidth(rotatedText->font, rotatedText->text)/2,
                    y - FC_GetHeight(rotatedText->font, rotatedText->text)/2,
                    NULL, 1.0f, 1.0f, angle, &pointOfRotation);
}