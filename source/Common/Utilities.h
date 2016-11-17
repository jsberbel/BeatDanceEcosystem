/* ========================================================================
   File: Utilities.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#if __APPLE__
#include <SDL2_image/SDL_image.h>
#else
#include <SDL_image.h>
#endif

#include <string>

#include "../Constants.h"
#include "Vector2D.h"

#define FILE_MAX_PATH 260

namespace Utils
{
	static void DoWrap(Vector2D& position)
	{
		if (position.x > FIXED_WIDTH)
		{
			position.x = 0.0f;
		}
		else if (position.x < 0)
		{
			position.x = FIXED_WIDTH;
		}

		if (position.y > FIXED_HEIGHT)
		{
			position.y = 0.0f;
		}
		else if (position.y < 0)
		{
			position.y = FIXED_HEIGHT;
		}
	}
}

namespace PathUtils
{
#define RESOURCE_FOLDER_NAME "resources"
	// Get the resources path
	static std::string GetResourcePath(const std::string &subDir = "") 
	{
		const char PATH_SEP = '/';
		
		// baseRes hold the base resource path
		// We give it static lifetime so that we'll only need to call
		// SDL_GetBasePath once to get the executable path
		static std::string baseRes;
		if (baseRes.empty()) 
		{
			// SDL_GetBasePath will return NULL if something went wrong in retrieving the path
			char *basePath = SDL_GetBasePath();
			if (basePath) 
			{
				baseRes = basePath;
				SDL_free(basePath);
			}
			else 
			{
				SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "Error getting resource path.\n");
				SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "SDL_GetError = %s.\n", SDL_GetError());
				return "";
			}

			// We replace the last bin/ with RESOURCE_FOLDER_NAME/ to get the the resource path
			size_t pos = baseRes.rfind("bin");
			baseRes = baseRes.substr(0, pos) + RESOURCE_FOLDER_NAME + PATH_SEP;
		}

		// If we want a specific subdirectory path in the resource directory
		// append it to the base path. This would be something like RESOURCE_FOLDER_NAME/Folder
		return subDir.empty() ? baseRes : baseRes + subDir + PATH_SEP;
	}
}

namespace CollisionUtils
{
	// SDL bounding box collision tests (works on SDL_Rect's)
	static int SDL_CollideBoundingBox(SDL_Rect a, SDL_Rect b)
	{
		if (b.x + b.w < a.x)	return 0;	// Checking if their
		if (b.x > a.x + a.w)	return 0;	// Bounding boxes even touch

		if (b.y + b.h < a.y)	return 0;
		if (b.y > a.y + a.h)	return 0;

		return 1;				// Bounding boxes intersect
	}
}


namespace IntegerUtils
{
	// Number of bits per byte
	#define CHAR_BIT 8

	inline int Abs(int value)
	{
		unsigned int r;
		int const mask = value >> (sizeof(int) * CHAR_BIT - 1);

		r = (value + mask) ^ mask;

		return r;
	}

	inline int Min(int value1, int value2)
	{
		return (value1 > value2 ? value2 : value1);
	}
	
	inline int Max(int value1, int value2)
	{
		return (value1 < value2 ? value2 : value1);
	}
	
	inline int Clamp(int clampValue, int minValue, int maxValue)
	{
		return Max(Min(clampValue, maxValue), minValue);
	}
	
	inline int RandomRange(int min, int max)
	{
		return (rand() % (max+1-min))+min;
	}


	inline int Sign(int value)
	{
		return (value > 0 ? 1 : -1);
	}
}

namespace FloatUtils
{
	inline float Remainder(float a, float b)
	{
		return (a - (floor(a / b) * b));
	}

	inline float Map(float x, float in_min, float in_max, float out_min, float out_max)
	{
		return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	}
	
	inline float Min(float value1, float value2)
	{
		return (value1 > value2 ? value2 : value1);
	}
	
	inline float Max(float value1, float value2)
	{
		return (value1 < value2 ? value2 : value1);
	}
	
	inline float Clamp(float clampValue, float minValue, float maxValue)
	{
		return Max(Min(clampValue, maxValue), minValue);
	}
	
	inline float Approach(float startPoint, float endPoint, float maxChange)
	{
		if (startPoint < endPoint)
		{
			return Min(startPoint + maxChange, endPoint);
		}
		else
		{
			return Max(startPoint - maxChange, endPoint);
		}
	}
	
	// Precondition max > min
	inline float RandomRange(float min, float max)
	{
		float random = ((float) rand()) / (float) RAND_MAX;
		float diff = max - min;
		float r = random * diff;
		return (min + r);
	}
	
	inline float RandomBinomial()
	{
		return ((float) rand() / (RAND_MAX)) - ((float) rand() / (RAND_MAX));
	}
	
	// Precise method which guarantees v = v1 when t = 1.
	inline float Lerp(float v0, float v1, float t)
	{
		return (1-t)*v0 + t*v1;
	}

	inline float CalculateOrientation(Vector2D direction)
	{
		return atan2f(direction.y, direction.x) * RAD2DEG;
	}
}

