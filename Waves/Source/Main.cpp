// ---------------------------------------------------------------------------
// includes

#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"
#include "Player.h"
#include "Maze.h"



// ---------------------------------------------------------------------------
// main

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);


	int gGameRunning = 1;

	// Initialization of your own variables go here
	//f32 tilesize = 100.0f;

	// Using custom window procedure
	AESysInit(hInstance, nCmdShow, 1600, 900, 1, 60, false, NULL);

	f32 tilesize = (((AEGfxGetWindowWidth() / 10.0f) < (AEGfxGetWindowHeight() / 10.0f)) ? (AEGfxGetWindowWidth() / 10.0f) : (AEGfxGetWindowHeight() / 10.0f));
	InitializePlayer(player, tilesize);

	AEGfxVertexList* pDonutMesh = CreateDonutMesh(1000.0f, 200.0f, 48, 0x00000000);
	AEGfxVertexList* PlayerMesh = Player_Mesh(48, 0x00000000, 10.0f);

	//change first parameter for tile size

	AEGfxVertexList* wallMesh = CreateSquareMesh(tilesize, 0xFF0000FF); // Blue for walls
	AEGfxVertexList* pathMesh = CreateSquareMesh(tilesize, 0xFFFFFFFF); // White for paths


	// Changing the window title
	AESysSetWindowTitle("My New Demo!");

	// reset the system modules
	AESysReset();

	//printf("Hello World\n");
	//f32 MoveX = 0.0f;
	//f32 MoveY = 0.0f;
	f32 dt = (f32)AEFrameRateControllerGetFrameTime();

	// Game Loop
	while (gGameRunning)
	{
		// Informing the system about the loop's start
		AESysFrameStart();
		AEGfxSetBackgroundColor(0.5, 0.5, 0.5);
		// Basic way to trigger exiting the application
		// when ESCAPE is hit or when the window is closed
		if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
			gGameRunning = 0;

		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		// Your own update logic goes here
		//player movement
		//if (AEInputCheckCurr(AEVK_W)){MoveY += 1000 * dt;}
		//if (AEInputCheckCurr(AEVK_S)){MoveY -= 1000 * dt;}
		//if (AEInputCheckCurr(AEVK_A)){MoveX -= 1000 * dt;}
		//if (AEInputCheckCurr(AEVK_D)){MoveX += 1000 * dt;}

		//if (AEInputCheckCurr(AEVK_W)) { PlayerMovement(playermove, 0.0f, 100.0f, dt, maze, tilesize); }
		//if (AEInputCheckCurr(AEVK_S)) { PlayerMovement(playermove, 0.0f, -100.0f, dt, maze, tilesize); }
		//if (AEInputCheckCurr(AEVK_A)) { PlayerMovement(playermove, -100.0f, 0.0f, dt, maze, tilesize); }
		//if (AEInputCheckCurr(AEVK_D)) { PlayerMovement(playermove, 100.0f, 0.0f, dt, maze, tilesize); }
		float speed = 10000.0f * dt; // Speed in units per second
		float dt = (float)AEFrameRateControllerGetFrameTime();

		if (AEInputCheckCurr(AEVK_W)) MovePlayer(player, 0.0f, 1.0f, speed, dt, maze, tilesize); // Move up
		if (AEInputCheckCurr(AEVK_S)) MovePlayer(player, 0.0f, -1.0f, speed, dt, maze, tilesize);  // Move down
		if (AEInputCheckCurr(AEVK_A)) MovePlayer(player, -1.0f, 0.0f, speed, dt, maze, tilesize); // Move left
		if (AEInputCheckCurr(AEVK_D)) MovePlayer(player, 1.0f, 0.0f, speed, dt, maze, tilesize);  // Move right

		// Your own rendering logic goes here
		RenderMaze(maze, tilesize, wallMesh, pathMesh); // 100.0f is the tile size
		RenderPlayer(player, PlayerMesh);
		//RenderMesh(PlayerMesh, player.x, player.y, 1.0f, 1.0f, 45.0f);
		RenderMesh(pDonutMesh, player.x, player.y, 1.0f, 1.0f, 45.0f);


		AEGfxSetCamPosition(player.x, player.y);
		// Informing the system about the loop's end
		AESysFrameEnd();
	}
	AEGfxMeshFree(pDonutMesh);

	// free the system
	AESysExit();
}