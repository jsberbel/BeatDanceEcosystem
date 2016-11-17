/* ========================================================================
   File: FlowFieldState.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "FlowFieldState.h"

#include <time.h>

void FlowFieldState::Init()
{
	// Initialize Fonts of different sizes
	SetResourcesFilePath("Fonts/nokiafc22.ttf");
	fontVerySmall = FC_CreateFont();
	FC_LoadFont(fontVerySmall, display->renderer, resourcesFilePath, 12, FC_MakeColor(255, 255, 255, 255), TTF_STYLE_NORMAL);
	fontSmall = FC_CreateFont();
	FC_LoadFont(fontSmall, display->renderer, resourcesFilePath, 22, FC_MakeColor(255, 255, 255, 255), TTF_STYLE_NORMAL);
	fontBig = FC_CreateFont();
	FC_LoadFont(fontBig, display->renderer, resourcesFilePath, 30, FC_MakeColor(255, 255, 255, 255), TTF_STYLE_NORMAL);

	// Initialize Textures
	boidTexture = {};
	SetResourcesFilePath("Images/Boid.png");
	if (Texture_LoadFromFile(&boidTexture, display->renderer, resourcesFilePath) == false)
	{
		SDL_Log("Failed to load Image!\n");
	}

	playerTexture = {};
	SetResourcesFilePath("Images/Player.png");
	if (Texture_LoadFromFile(&playerTexture, display->renderer, resourcesFilePath) == false)
	{
		SDL_Log("Failed to load Image!\n");
	}

	// Initialize Entities
	player = {};
	player.position = { FIXED_WIDTH / 2,  FIXED_HEIGHT / 2 };
	player.width = playerTexture.width;
	player.height = playerTexture.height;

	// Seed Random
	srand(time(NULL));
	int randomX = IntegerUtils::RandomRange(0, FIXED_WIDTH);
	int randomY = IntegerUtils::RandomRange(0, FIXED_HEIGHT);

	// Initialize Flow Field
	FlowField_Init(&field);

	// Initialize Random Obstacles

	//int randomRadius;
	//for (int i = 0; i < FLOW_OBSTACLE_POOL_SIZE; ++i)
	//{
	//	randomX = IntegerUtils::RandomRange(0, GRID_SIZE);
	//	randomY = IntegerUtils::RandomRange(0, GRID_SIZE);
	//	randomRadius = 1;  //IntegerUtils::RandomRange(1, 4);
	//	FlowObstacle_Init(&obstaclePool[i], 
	//	{ (float)(randomX * (FIXED_WIDTH / GRID_SIZE) + (FIXED_WIDTH / GRID_SIZE) / 2),
	//		(float)(randomY * (FIXED_HEIGHT / GRID_SIZE) + (FIXED_HEIGHT / GRID_SIZE) / 2) },
	//		(float)(randomRadius * GRID_SIZE));
	//	FlowField_AddObstacle(&field, &obstaclePool[i]);
	//	++obstaclePoolOccupation;
	//}

	// Boids
	for (int i = 0; i < INITIAL_NUMBER_BOIDS; ++i)
	{
		randomX = IntegerUtils::RandomRange(0, FIXED_WIDTH);
		randomY = IntegerUtils::RandomRange(0, FIXED_HEIGHT);
		CreateBoid(randomX, randomY, Behaviour::FLOW_FLOCK_SEEK_PRIORITY);
	}

	// Initialize Text
	title = (char*)"Flocking + Seek + Flow Field Obstacle Avoidance";
	theoricUpdateInformation = (char*)"";
	theroicAlgorithmInformation = (char*)"";
}

void FlowFieldState::Deinit()
{
	boidPool[0].DeinitDebug();
	Texture_Free(&boidTexture);
	Texture_Free(&playerTexture);
	FC_FreeFont(fontVerySmall);
	FC_FreeFont(fontSmall);
	FC_FreeFont(fontBig);
}

StateCode FlowFieldState::HandleInput()
{
	if (InputSingleton::Instance()->doQuit)
	{
		return StateCode::QUIT;
	}
	// keyboard Input Handling
	else if (InputSingleton::Instance()->IsKeyDown(SDL_SCANCODE_ESCAPE))
	{
		return StateCode::MENU;
	}
	else if (InputSingleton::Instance()->IsKeyDown(SDL_SCANCODE_SPACE))
	{
		paused = !paused;
	}
	else if (InputSingleton::Instance()->IsKeyDown(SDL_SCANCODE_R))
	{
		boidPoolOccupation = 0;
	}
	else if (InputSingleton::Instance()->IsKeyDown(SDL_SCANCODE_S))
	{
		// Take out smoothing
		for (int i = 0; i < boidPoolOccupation; i++)
		{
			boidPool[i].smoothAngleChange = !boidPool[i].smoothAngleChange;
		}
	}

	// Mouse Input Handling
	if (InputSingleton::Instance()->mouseButtonState)
	{
		//SDL_Log("Create Boid!");
		CreateBoid(InputSingleton::Instance()->mousePosition.x, 
			InputSingleton::Instance()->mousePosition.y, boidPool[0].currentBehaviour);
	}

	if (InputSingleton::Instance()->mouseSecondaryButtonState)
	{
		//SDL_Log("Create Obstacle!");
		CreateObstacle(InputSingleton::Instance()->mousePosition);
	}

	// Gamepad Input Handling
	if (InputSingleton::Instance()->gamepadButtonsState.start.isDown 
		&& !InputSingleton::Instance()->gamepadButtonsState.start.wasDown)
	{
		paused = !paused;
	}

	player.HandleInput();

	return StateCode::CONTINUE;
}

void FlowFieldState::Update(float deltaTime)
{
	// Pause management
	if (paused == true)
	{
		return;
	}

	// Calculate time
	float currentDeltaTime = deltaTime;

	// Update Entities
	player.Update(currentDeltaTime);

	// Update Boids in Pool
	for (int i = 0; i < boidPoolOccupation; i++)
	{
		boidPool[i].Update(currentDeltaTime);
	}

	// Store Mouse Position
	sprintf(mouseInformation, "(%.1f, %.1f)",
		InputSingleton::Instance()->mousePosition.x,
		InputSingleton::Instance()->mousePosition.y);
	sprintf(smoothInformation, "Smooth Angle is %s",
		boidPool[0].smoothAngleChange ? "ON" : "OFF");
	
}

void FlowFieldState::Render()
{
	// Render Flow Obstacles
	for (int i = 0; i < obstaclePoolOccupation; ++i)
	{
		FlowObstacle_Render(&obstaclePool[i], display->renderer, Colors::TURQUOISE);
	}

	// Render Flow Field
	FlowField_Render(&field, display->renderer, Colors::BELIZE_HOLE);

	// Render Boids in Pool
	for (int i = 0; i < boidPoolOccupation; ++i)
	{
		boidPool[i].Render(&boidTexture, display->renderer);
	}

	// Render Entities
	player.Render(&playerTexture, display->renderer);

	// Render Text
	FC_DrawAlign(fontBig, display->renderer,
		0,
		FIXED_HEIGHT - 140,
		FC_ALIGN_LEFT,
		title);

	FC_DrawAlign(fontSmall, display->renderer,
		FIXED_WIDTH,
		0,
		FC_ALIGN_RIGHT,
		mouseInformation);
	FC_DrawAlign(fontSmall, display->renderer,
		0,
		FIXED_HEIGHT - 40,
		FC_ALIGN_LEFT,
		smoothInformation);
	FC_DrawAlign(fontSmall, display->renderer,
		0,
		FIXED_HEIGHT - 160,
		FC_ALIGN_LEFT,
		theroicAlgorithmInformation);

	if (isPaused == true)
	{
		FC_DrawAlign(fontSmall, display->renderer,
			0,
			0,
			FC_ALIGN_LEFT,
			(char*)"Pause");
	}
}

void FlowFieldState::ChangeBoidsBehavior(Behaviour newBehavior)
{
	for (int i = 0; i < boidPoolOccupation; i++)
	{
		boidPool[i].SetBehaviour(newBehavior);
	}
}

void FlowFieldState::CreateBoid(int x, int y, Behaviour behaviour)
{
	if (boidPoolOccupation < BOID_POOL_SIZE)
	{
		boidPool[boidPoolOccupation] = {};
		boidPool[boidPoolOccupation].position = { (float)x, (float)y };
		boidPool[boidPoolOccupation].width = boidTexture.width;
		boidPool[boidPoolOccupation].height = boidTexture.height;

		// Random Speed
		float randomX = IntegerUtils::RandomRange(-30, 30);
		float randomY = IntegerUtils::RandomRange(-30, 30);
		boidPool[boidPoolOccupation].speed = { (float)randomX , (float)randomY };

		boidPool[boidPoolOccupation].target = &player;
		boidPool[boidPoolOccupation].SetBehaviour(behaviour);

		// Configure Flocking parameters
		boidPool[boidPoolOccupation].boidArray = (Boid*)boidPool;
		boidPool[boidPoolOccupation].boidArraySize = &boidPoolOccupation;

		// Configure Flow Field parameters
		boidPool[boidPoolOccupation].field = &field;
		
		// Set initial speed
		randomX = IntegerUtils::RandomRange(-30, 30);
		randomY = IntegerUtils::RandomRange(-30, 30);
		boidPool[boidPoolOccupation].speed = { (float)randomX , (float)randomY };

		// Set Smooth Angle Change
		boidPool[boidPoolOccupation].smoothAngleChange = false;

		// Init Priority
		boidPool[boidPoolOccupation].InitPrioritySteering();

		++boidPoolOccupation;
	}
	else
	{
		SDL_Log("Cannot create Boid. Pool is Full!");
	}
}

void FlowFieldState::CreateObstacle(Vector2D position)
{
	if (obstaclePoolOccupation < FLOW_OBSTACLE_POOL_SIZE)
	{
		int obstacleRadius = 1;  //IntegerUtils::RandomRange(1, 4);

		int obstacleX = (position.x / (FIXED_WIDTH / FIELD_SIZE));
		int obstacleY = (position.y / (FIXED_HEIGHT / FIELD_SIZE));

		FlowObstacle_Init(&obstaclePool[obstaclePoolOccupation],
		{ (float)(obstacleX * (FIXED_WIDTH / FIELD_SIZE) + (FIXED_WIDTH / FIELD_SIZE) / 2),
			(float)(obstacleY * (FIXED_HEIGHT / FIELD_SIZE) + (FIXED_HEIGHT / FIELD_SIZE) / 2) },
			(float)(obstacleRadius * FIELD_SIZE));
		FlowField_AddObstacle(&field, &obstaclePool[obstaclePoolOccupation]);
		++obstaclePoolOccupation;
	}
	else
	{
		SDL_Log("Cannot create Obstacle. Pool is Full!");
	}
}

