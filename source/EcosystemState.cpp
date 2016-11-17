/* ========================================================================
   File: EcosystemState.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "EcosystemState.h"
#include <time.h>

void EcosystemState::Init()
{
	// Initialize Fonts of different sizes
	SetResourcesFilePath("Fonts/nokiafc22.ttf");
	fontVerySmall = FC_CreateFont();
	FC_LoadFont(fontVerySmall, display->renderer, resourcesFilePath, 12, FC_MakeColor(20, 20, 20, 255), TTF_STYLE_NORMAL);
	fontSmall = FC_CreateFont();
	FC_LoadFont(fontSmall, display->renderer, resourcesFilePath, 22, FC_MakeColor(20, 20, 20, 255), TTF_STYLE_NORMAL);
	fontBig = FC_CreateFont();
	FC_LoadFont(fontBig, display->renderer, resourcesFilePath, 30, FC_MakeColor(20, 20, 20, 255), TTF_STYLE_NORMAL);

	// Initialize Textures
	boidTexture = {};
	SetResourcesFilePath("Images/Boid.png");
	if (Texture_LoadFromFile(&boidTexture, display->renderer, resourcesFilePath) == false) {
		SDL_Log("Failed to load Image!\n");
	}

	playerTexture = {};
	SetResourcesFilePath("Images/Player.png");
	if (Texture_LoadFromFile(&playerTexture, display->renderer, resourcesFilePath) == false) {
		SDL_Log("Failed to load Image!\n");
	}

	groundTexture = {};
	SetResourcesFilePath("Images/DiscoFloor.png");
	if (Texture_LoadFromFile(&groundTexture, display->renderer, resourcesFilePath) == false) {
		SDL_Log("Failed to load Image!\n");
	}

	barTexture = {};
	SetResourcesFilePath("Images/WhiteBar.png");
	if (Texture_LoadFromFile(&barTexture, display->renderer, resourcesFilePath) == false) {
		SDL_Log("Failed to load Image!\n");
	}

	// Initialize Entities
	player = {};
	player.position = { FIXED_WIDTH / 2,  FIXED_HEIGHT / 2 };
	player.width = playerTexture.width;
	player.height = playerTexture.height;

	// Initialize Flow Field
	FlowField_Init(&field);

	// Seed Random
	srand(time(NULL));
	int randomX = IntegerUtils::RandomRange(0, FIXED_WIDTH);
	int randomY = IntegerUtils::RandomRange(0, FIXED_HEIGHT);

	// Boids
	curBehavior = Behaviour::WANDER;
	for (int i = 0; i < INITIAL_BOIDS; ++i) {
		randomX = IntegerUtils::RandomRange(0, FIXED_WIDTH);
		randomY = IntegerUtils::RandomRange(0, FIXED_HEIGHT);
		CreateBoid(randomX, randomY, curBehavior);
	}

	// Initialize Text
	title = (char*)"XX Disco Dance Ecosystem";
	theoricUpdateInformation = (char*)"";
	theroicAlgorithmInformation = (char*)"";

	// Initialize songs and beat detector
	beatDetector = BeatDetector::Instance();
	beatDetector->loadSystem();
	
	AddSong("Music/BillieJean.mp3");
	AddSong("Music/AnotherOneBitesTheDust.mp3");
	AddSong("Music/UCantTouchThis.mp3");
	AddSong("Music/StayinAlive.mp3");
	AddSong("Music/DiscoInferno.mp3");
	AddSong("Music/NeverGonnaGiveYouUp.mp3");
	AddSong("Music/BoysBoysBoys.mp3");
	AddSong("Music/Maniac.mp3");
	AddSong("Music/BackInBlack.mp3");
	AddSong("Music/DaddyCool.mp3");
	PlaySong(0, 1.0f);

	gameOver = false;
	strcpy(scoreInformation, "");
	timeCounter = clock();
}

void EcosystemState::Deinit()
{
	boidPool[0].DeinitDebug();
	Texture_Free(&boidTexture);
	Texture_Free(&playerTexture);
	FC_FreeFont(fontVerySmall);
	FC_FreeFont(fontSmall);
	FC_FreeFont(fontBig);
}

StateCode EcosystemState::HandleInput()
{
	if (InputSingleton::Instance()->doQuit) {
		return StateCode::QUIT;
	}
	// keyboard Input Handling
	else if (InputSingleton::Instance()->IsKeyDown(SDL_SCANCODE_ESCAPE) || gameOver) {
		beatDetector->setStarted(false);
		return StateCode::MENU;
	} else if (InputSingleton::Instance()->IsKeyDown(SDL_SCANCODE_SPACE)) {
		beatDetector->setStarted(paused);
		paused = !paused;
	} else if (InputSingleton::Instance()->IsKeyDown(SDL_SCANCODE_S)) {
		// Take out smoothing
		for (int i = 0; i < boidPoolOccupation; i++) {
			boidPool[i].smoothAngleChange = !boidPool[i].smoothAngleChange;
		}
	}
	else if (InputSingleton::Instance()->IsKeyDown(SDL_SCANCODE_0)) PlaySong(0, 1.0f), curBehavior = Behaviour::WANDER;
	else if (InputSingleton::Instance()->IsKeyDown(SDL_SCANCODE_1)) PlaySong(1, 2.0f), curBehavior = Behaviour::FLOW_FLOCK_SEEK_PRIORITY;
	else if (InputSingleton::Instance()->IsKeyDown(SDL_SCANCODE_2)) PlaySong(2, 1.2f), curBehavior = Behaviour::WANDER;
	else if (InputSingleton::Instance()->IsKeyDown(SDL_SCANCODE_3)) PlaySong(3, 1.4f), curBehavior = Behaviour::WANDER;
	else if (InputSingleton::Instance()->IsKeyDown(SDL_SCANCODE_4)) PlaySong(4, 2.0f), curBehavior = Behaviour::WANDER;
	else if (InputSingleton::Instance()->IsKeyDown(SDL_SCANCODE_5)) PlaySong(5, 1.8f), curBehavior = Behaviour::WANDER;
	else if (InputSingleton::Instance()->IsKeyDown(SDL_SCANCODE_6)) PlaySong(6, 3.5f), curBehavior = Behaviour::FLOW_FLOCK_SEEK_PRIORITY;
	else if (InputSingleton::Instance()->IsKeyDown(SDL_SCANCODE_7)) PlaySong(7, 2.0f), curBehavior = Behaviour::WANDER;
	else if (InputSingleton::Instance()->IsKeyDown(SDL_SCANCODE_8)) PlaySong(8, 2.0f), curBehavior = Behaviour::WANDER;
	else if (InputSingleton::Instance()->IsKeyDown(SDL_SCANCODE_9)) PlaySong(9, 3.0f), curBehavior = Behaviour::WANDER;

	// Gamepad Input Handling
	if (InputSingleton::Instance()->gamepadButtonsState.start.isDown
		&& !InputSingleton::Instance()->gamepadButtonsState.start.wasDown) {
		paused = !paused;
	}

	player.HandleInput();

	return StateCode::CONTINUE;
}

void EcosystemState::Update(float deltaTime)
{
	// Pause management
	if (paused == true) {
		return;
	}

	// Update Entities
	player.Update(deltaTime);

	if (curBehavior != prevBehavior) ChangeBoidsBehavior(curBehavior);

	// Update Boids in Pool
	if (danceDeltaTime > deltaTime) danceDeltaTime *= 0.9f;

	if (lastBeat != beatDetector->getLastBeat()) {
		danceDeltaTime = deltaTime + lastBeat->getFrequency()*danceMult*0.008f;
		lastBeat = beatDetector->getLastBeat();
		if (IntegerUtils::RandomRange(0, 10) < 1) danceDeltaTime = -deltaTime, reverseCounter = 0;
		++gameScore;
 	}

	if (clock() > timeCounter + SPAWN_TIME) {
		CreateBoid(IntegerUtils::RandomRange(0, FIXED_WIDTH), IntegerUtils::RandomRange(0, FIXED_HEIGHT), curBehavior);
		timeCounter = clock();
	}

	if (reverseCounter > 20 && danceDeltaTime < 0.0f) danceDeltaTime = deltaTime;
	else ++reverseCounter;

	for (int i = 0; i < boidPoolOccupation; i++) {
		boidPool[i].Update(danceDeltaTime);
	}

	// Store Mouse Position
	sprintf(mouseInformation, "(%.1f, %.1f)",
			InputSingleton::Instance()->mousePosition.x,
			InputSingleton::Instance()->mousePosition.y);
	sprintf(smoothInformation, "Smooth Angle is %s",
			boidPool[0].smoothAngleChange ? "ON" : "OFF");
	sprintf(scoreInformation, "Beat Score: %i", gameScore);
	sprintf(enemiesInformation, "Evil dancers: %i", boidPoolOccupation);

	// Update Beat Detector
	beatDetector->update();

	prevBehavior = curBehavior;

	if (CheckCollision()) {
		beatDetector->setStarted(false); 
		gameOver = true;
	}
}

void EcosystemState::Render()
{
	Texture_Render(&groundTexture, display->renderer,
				   0,
				   0,
				   nullptr, 1, 1, 0);

	Texture_Render(&barTexture, display->renderer,
				   0,
				   -50,
				   nullptr, 1, 1, 0);

	Texture_Render(&barTexture, display->renderer,
				   0,
				   FIXED_HEIGHT - 130,
				   nullptr, 1, 2, 0);

	// Render Boids in Pool
	for (int i = 0; i < boidPoolOccupation; ++i) {
		boidPool[i].Render(&boidTexture, display->renderer);
	}

	// Render Entities
	player.Render(&playerTexture, display->renderer);

	// Render Text
	FC_DrawAlign(fontBig, display->renderer,
				 0,
				 FIXED_HEIGHT - 120,
				 FC_ALIGN_LEFT,
				 title);
	FC_DrawAlign(fontSmall, display->renderer,
				 0,
				 FIXED_HEIGHT - 70,
				 FC_ALIGN_LEFT,
				 "Press numbers 0 to 9 to change song/level.");
	FC_DrawAlign(fontSmall, display->renderer,
				 0,
				 FIXED_HEIGHT - 30,
				 FC_ALIGN_LEFT,
				 "Avoid the deadly dancers to survive.");

	FC_DrawAlign(fontSmall, display->renderer,
				 0,
				 0,
				 FC_ALIGN_LEFT,
				 scoreInformation);
	FC_DrawAlign(fontSmall, display->renderer,
				 FIXED_WIDTH,
				 0,
				 FC_ALIGN_RIGHT,
				 enemiesInformation);

	if (isPaused == true) {
		FC_DrawAlign(fontSmall, display->renderer,
					 0,
					 0,
					 FC_ALIGN_LEFT,
					 (char*)"Pause");
	}
}

bool EcosystemState::CheckCollision() const {
	for (int i = 0; i < boidPoolOccupation; i++) {
		if (player.position.x <= boidPool[i].position.x + boidPool[i].width*0.5f && player.position.x >= boidPool[i].position.x - boidPool[i].width*0.5f &&
			player.position.y <= boidPool[i].position.y + boidPool[i].height*0.5f && player.position.y >= boidPool[i].position.y - boidPool[i].height*0.5f)
			return true;
	}
	return false;
}

void EcosystemState::PlaySong(int num, int mult) {
	beatDetector->setStarted(false);
	danceMult = mult;
	beatDetector->LoadSong(SAMPLE_SIZE, (char*)songNames[num]);
	beatDetector->setStarted(true);
	lastBeat = beatDetector->getLastBeat();
}

void EcosystemState::AddSong(char* path) {
	if (songOccupation < MAX_SONGS) {
		strcpy(songNames[songOccupation], path);
		++songOccupation;
	} else {
		SDL_Log("Cannot add song. Max capacity exceeded!");
	}
}

void EcosystemState::ChangeBoidsBehavior(Behaviour newBehavior) {
	for (int i = 0; i < boidPoolOccupation; i++) {
		boidPool[i].SetBehaviour(newBehavior);
	}
}

void EcosystemState::CreateBoid(int x, int y, Behaviour behaviour) {
	if (boidPoolOccupation < MAX_BOIDS) {
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
}