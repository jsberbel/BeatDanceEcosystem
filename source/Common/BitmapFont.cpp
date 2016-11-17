/* ========================================================================
   File: BitmapFont.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "BitmapFont.h"

void BitmapFont_Free(BitmapFont* bitmapFont)
{
    Texture_UnlockTexture(bitmapFont->bitmap);
    Texture_Free(bitmapFont->bitmap);
}

bool BitmapFont_BuildFont(BitmapFont* bitmapFont, Texture* bitmap)
{
    bool success = true;
    
    if (Texture_LockTexture(bitmap) == false)
    {
        printf("Unable to lock bitmap font texture\n");
        success = false;
    }
    else
    {
        // Set the background color
        Uint32 bgColor = Texture_GetPixel32(bitmap, 0, 0);
        
        // Set the cell dimensions
        int cellW = bitmap->width / 14;
        int cellH = bitmap->height / 14;
        
        // New line variables
        int top = cellH;
        int baseA = cellH;
        
        int currentChar = 0;
        
        for(int rows = 0; rows < 16; ++rows)
        {
            for(int cols = 0; cols < 16; ++cols)
            {
                // Set the character offset
                bitmapFont->chars[currentChar].x = cellW * cols;
                bitmapFont->chars[currentChar].y = cellH * rows;
                
                // Set the dimensions of the character
                bitmapFont->chars[currentChar].w = cellW;
                bitmapFont->chars[currentChar].h = cellH;
                
                // Find Left Side
                // Go through pixel columns
                for(int pCol = 0; pCol < cellW; ++pCol)
                {
                    // Go through pixel rows
                    for(int pRow = 0; pRow < cellH; ++pRow)
                    {
                        // Get the pixel offsets
                        int pX = (cellW * cols) + pCol;
                        int pY = (cellH * rows) + pRow;
                        
                        if (Texture_GetPixel32(bitmap, pX, pY) != bgColor)
                        {
                            // Set x offset
                            bitmapFont->chars[currentChar].x = pX;
                            
                            // Break the loops
                            pCol = cellW;
                            pRow = cellH;
                        }
                    }
                }
                
                // Find Right Side
                // Go through pixel column
                for(int pColW = cellW - 1; pColW >= 0; --pColW)
                {
                    //Go through pixel rows
                    for(int pRowW = 0; pRowW < cellH; ++pRowW)
                    {
                        // Get the pixel offsets
                        int pX = (cellW * cols) + pColW;
                        int pY = (cellH * rows) + pRowW;
                        
                        if (Texture_GetPixel32(bitmap, pX, pY) != bgColor)
                        {
                            // Set the width
                            bitmapFont->chars[currentChar].w = (pX - bitmapFont->chars[currentChar].x) + 1;
                            
                            // Break the loops
                            pColW = -1;
                            pRowW = cellH;
                        }
                    }
                }
                
                //Find Top
                //Go through pixel rows
                for( int pRow = 0; pRow < cellH; ++pRow )
                {
                    //Go through pixel columns
                    for( int pCol = 0; pCol < cellW; ++pCol )
                    {
                        //Get the pixel offsets
                        int pX = ( cellW * cols ) + pCol;
                        int pY = ( cellH * rows ) + pRow;
                        
                        //If a non colorkey pixel is found
                        if( Texture_GetPixel32(bitmap, pX, pY) != bgColor )
                        {
                            //If new top is found
                            if( pRow < top )
                            {
                                top = pRow;
                            }
                            
                            //Break the loops
                            pCol = cellW;
                            pRow = cellH;
                        }
                    }
                }
                
                //Find Bottom of A
                if( currentChar == 'A' )
                {
                    //Go through pixel rows
                    for( int pRow = cellH - 1; pRow >= 0; --pRow )
                    {
                        //Go through pixel columns
                        for( int pCol = 0; pCol < cellW; ++pCol )
                        {
                            //Get the pixel offsets
                            int pX = ( cellW * cols ) + pCol;
                            int pY = ( cellH * rows ) + pRow;
                            
                            //If a non colorkey pixel is found
                            if( Texture_GetPixel32(bitmap, pX, pY)  != bgColor )
                            {
                                //Bottom of a is found
                                baseA = pRow;
                                
                                //Break the loops
                                pCol = cellW;
                                pRow = -1;
                            }
                        }
                    }
                }
                
                //Go to the next character
                ++currentChar;
            }
        }
        //Calculate space
        bitmapFont->space = cellW / 2;
        
        //Calculate new line
        bitmapFont->newLine = baseA - top;
        
        //Lop off excess top pixels
        for( int i = 0; i < 256; ++i )
        {
            bitmapFont->chars[ i ].y += top;
            bitmapFont->chars[ i ].h -= top;
        }
        
        Texture_UnlockTexture(bitmap);
        bitmapFont->bitmap = bitmap;
    }
    
    return success;
}

void BitmapFont_RenderText(BitmapFont* bitmapFont, SDL_Renderer* renderer, int x, int y, std::string text)
{
    //If the font has been built
    if( bitmapFont->bitmap != NULL )
    {
        //Temp offsets
        int curX = x, curY = y;
        
        //Go through the text
        for( int i = 0; i < (int)text.length(); ++i )
        {
            //If the current character is a space
            if( text[ i ] == ' ' )
            {
                //Move over
                curX += bitmapFont->space;
            }
            //If the current character is a newline
            else if( text[ i ] == '\n' )
            {
                //Move down
                curY += bitmapFont->newLine;
                
                //Move back
                curX = x;
            }
            else
            {
                //Get the ASCII value of the character
                int ascii = (unsigned char)text[ i ];
                
                //Show the character
                Texture_Render(bitmapFont->bitmap, renderer, curX, curY, &bitmapFont->chars[ ascii ] );
                
                //Move over the width of the character with one pixel of padding
                curX += bitmapFont->chars[ ascii ].w + 1;
            }
        }
    }
}

