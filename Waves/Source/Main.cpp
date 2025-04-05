/* Start Header ************************************************************************/
/*!
\file		Main.cpp
\author		Henrey Hu
\email		zhihenghenrey.hu@digipen.edu

\brief		This file contains the entry point for the game, initializing the Alpha Engine system and managing the game loop. 
			It sets up the GameStateManager, starts in the main menu, and continuously updates and renders the game until the application exits.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

// ---------------------------------------------------------------------------
// includes

#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"

#include "GSM.h"
#include "mainMenu.h"
#include "splash.h"


// ---------------------------------------------------------------------------
// main
u32 white_colour = 0xFFFFFFFF;
u32 red_colour = 0xFFFF0000; // Full opacity, full red, no green, no blue

//f32 tilesize;
AEGfxVertexList* pDonutMesh;
AEGfxVertexList* PlayerMesh;
AEGfxVertexList* wallMesh;
AEGfxVertexList* pathMesh;
AEGfxVertexList* enemyMesh;
//printf("Hello World\n");
f32 MoveX;
f32 MoveY;
f32 dt;
//
Enemy enemy;

void init() {
	// tilesize = 100.0f;
	pDonutMesh = CreateDonutMesh(1000.0f, 200.0f, 48, 0x00000000);
	PlayerMesh = Player_Mesh(48, 0x00000000, 10.0f);
	enemyMesh = createEnemy_mesh(red_colour);
	//change first parameter for tile size
	if (!enemyMesh) {
		printf("error enemy mesh");
	}

	wallMesh = CreateSquareMesh(TILE_SIZE, 0xFF0000FF); // Blue for walls
	pathMesh = CreateSquareMesh(TILE_SIZE, 0xFFFFFFFF); // White for paths
	//printf("Hello World\n");
	 MoveX = 0.0f;
	 MoveY = 0.0f;


	 enemy.x = 100.f; //starting position x
	 enemy.y = -100.f; //starting position y
	 enemy.w = 50.f;
	 enemy.h = 50.f;	
}

void update() {


	//player movement
	//if (AEInputCheckCurr(AEVK_W)){MoveY += 1000 * dt;}
	//if (AEInputCheckCurr(AEVK_S)){MoveY -= 1000 * dt;}
	//if (AEInputCheckCurr(AEVK_A)){MoveX -= 1000 * dt;}
	//if (AEInputCheckCurr(AEVK_D)){MoveX += 1000 * dt;}

	//if (AEInputCheckCurr(AEVK_W)) { PlayerMovement(playermove, 0.0f, 100.0f, dt, maze, tilesize); }
	//if (AEInputCheckCurr(AEVK_S)) { PlayerMovement(playermove, 0.0f, -100.0f, dt, maze, tilesize); }
	//if (AEInputCheckCurr(AEVK_A)) { PlayerMovement(playermove, -100.0f, 0.0f, dt, maze, tilesize); }
	//if (AEInputCheckCurr(AEVK_D)) { PlayerMovement(playermove, 100.0f, 0.0f, dt, maze, tilesize); }

	//float dt = (float)AEFrameRateControllerGetFrameTime();
	dt = (f32)AEFrameRateControllerGetFrameTime();
	static float accumulatedTime = 0.0f;
	const float updateInterval = 0.1f;  // Move every 0.1 seconds
	// get dt for this frame
	accumulatedTime += dt;
	float speed = 250.0f; // Speed in units per second

	if (AEInputCheckCurr(AEVK_W)) MovePlayer(player, 0.0f, 1.0f, speed, dt, maze, TILE_SIZE); // Move up
	if (AEInputCheckCurr(AEVK_S)) MovePlayer(player, 0.0f, -1.0f, speed, dt, maze, TILE_SIZE);  // Move down
	if (AEInputCheckCurr(AEVK_A)) MovePlayer(player, -1.0f, 0.0f, speed, dt, maze, TILE_SIZE); // Move left
	if (AEInputCheckCurr(AEVK_D)) MovePlayer(player, 1.0f, 0.0f, speed, dt, maze, TILE_SIZE);  // Move right


	Pair src = {1, 1}, dest = { 8, 8 };

	if (!enemy.reachPath) {  // Only calculate path once
		astarsearch(maze, src, dest, enemy.path);
		enemy.reachPath = true;  //  Mark path as calculated
	}

	// Move enemy if enough time has passed
	if (accumulatedTime >= updateInterval) {
		if (enemy.pathIndex < enemy.path.size()) {
			enemy.moveEnemy();
			enemy.printPosition(); 
		}
		accumulatedTime = 0.0f; 
	}

}

void draw() {
	RenderMaze(maze, TILE_SIZE, wallMesh, pathMesh); // 100.0f is the tile size
	//RenderPlayer(player, PlayerMesh);
	//RenderEnemy_Mesh(enemyMesh, enemy.x, enemy.y, enemy.w, enemy.h, enemy.)

	//RenderMesh(enemyMesh, enemy.x, enemy.y, 1.0f, 1.0f, 0.f);
	//RenderMesh(PlayerMesh, player.x, player.y, 1.0f, 1.0f, 45.0f);
	//RenderMesh(pDonutMesh, player.x, player.y, 1.0f, 1.0f, 45.0f);
	if (!enemyMesh) {
		printf("Error: enemyMesh is nullptr\n");
	}
	else {
		RenderMesh(enemyMesh, enemy.x, enemy.y, enemy.w, enemy.h, 0.f);
	}

	AEGfxSetCamPosition(player.x, player.y);
	// Informing the system about the loop's end
}

void free(){
	//AEGfxVertexList* pDonutMesh;
	//AEGfxVertexList* PlayerMesh;
	//AEGfxVertexList* wallMesh;
	//AEGfxVertexList* pathMesh;
	AEGfxMeshFree(pDonutMesh);
	AEGfxMeshFree(PlayerMesh);
	AEGfxMeshFree(wallMesh);
	AEGfxMeshFree(pathMesh);
	AEGfxMeshFree(enemyMesh);
	
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// Using custom window procedure
	AESysInit(hInstance, nCmdShow, 1600, 900, 1, 60, false, NULL);

	// Changing the window title
	AESysSetWindowTitle("Waves");

	int gGameRunning = 1;

	// Initialize fonts globally
	FontManager::InitializeFonts();

	AEAudioGroup keyAudio = AEAudioCreateGroup();  // For background music
	AudioManager::LoadAudio("GameOver", "Assets/Audio/GameOver.wav", false, keyAudio);
	AudioManager::LoadAudio("Win", "Assets/Audio/Win.wav", false, keyAudio);

	GameStateManager gameStateManager;
	gameStateManager.SetGameRunningPtr(&gGameRunning); // Provide the pointer to GSM

	//// Create main menu
	//MainMenuState* menu = new MainMenuState();
	//gameStateManager.SetState(menu);

	// Start with the splash state
	SplashState* splash = new SplashState();
	gameStateManager.SetState(splash);

	// reset the system modules
	AESysReset();
	
	// Game Loop
	while (gGameRunning)
	{
		// Informing the system about the loop's start
		AESysFrameStart();
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);

		f32 dt = (f32)AEFrameRateControllerGetFrameTime();
		gameStateManager.Update(dt);
		gameStateManager.Draw();

		if (0 == AESysDoesWindowExist()) {
			gGameRunning = 0;
		}

		// Informing the system about the loop's end
		AESysFrameEnd();
	}

	// Shutdown audio and free fonts
	FontManager::ShutdownFonts();
	AudioManager::ShutdownAllAudio();

	// free the system
	AESysExit();
}