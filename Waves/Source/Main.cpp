// ---------------------------------------------------------------------------
// includes

#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"
#include "draw_map.h"
#include "enemy_henrey.h"
#include "Player.h"
#include <cstdlib>



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

	// Using custom window procedure
	AESysInit(hInstance, nCmdShow, 1600, 900, 1, 60, false, NULL);


	AEGfxVertexList* wallMesh = createWallMesh(0xFF008080);
	AEGfxVertexList* boundaryMesh = createBoundaryMesh(0xFF654321);
	AEGfxVertexList* enemy1 = createEnemyMesh(0xFFFF0000);

	addWall(0.0f, 0.0f, 600.0f, 300.0f);
	addBoundary(0.0f, 300.0f, 1000.0f, 50.0f);
	addBoundary(-520.0f, 0.0f, 50.0f, 650.0f);
	addBoundary(0.0f, -300.0f, 1000.0f, 50.0f);
	addBoundary(520.0f, 0.0f, 50.0f, 650.0f);

	// Test the walls array
	for (int i = 0; i < wallCount; i++) {
		printf("Wall %f: x=%f, y=%f, width=%f, height=%f\n", i, wallList[i].x, wallList[i].y, wallList[i].scale_x, wallList[i].scale_y);
	}

	// Test the boundaries array
	for (int i = 0; i < boundaryCount; i++) {
		printf("Boundary %f: x=%f, y=%f, width=%f, height=%f\n", i, boundaryList[i].x, boundaryList[i].y, boundaryList[i].scale_x, boundaryList[i].scale_y);
	}
	AEGfxVertexList* pDonutMesh = CreateDonutMesh(1000.0f, 200.0f, 48, 0x00000000);
	AEGfxVertexList* PlayerMesh = Player_Mesh(48, 0x00000000, 10.0f);
	AEGfxVertexList* testing = testing_only(); //to delete for testing only

	// Changing the window title
	AESysSetWindowTitle("My New Demo!");

	// reset the system modules
	AESysReset();

	//printf("Hello World\n");
	f32 MoveX = 0.0f;
	f32 MoveY = 0.0f;
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
		if (AEInputCheckCurr(AEVK_W))
		{
			MoveY += 1000*dt;
		}
		if (AEInputCheckCurr(AEVK_S))
		{
			MoveY -= 1000 * dt;
		}
		if (AEInputCheckCurr(AEVK_A))
		{
			MoveX -= 1000 * dt;
		}
		if (AEInputCheckCurr(AEVK_D))
		{
			MoveX += 1000 * dt;
		}

		// Your own rendering logic goes here
		drawWalls(wallMesh);
		drawBoundaries(boundaryMesh);
		RenderMesh(enemy1, 450.0f, -200.0f, 50.0f, 50.0f, 0.0f);
		//drawing the donut mesh
		RenderMesh(PlayerMesh, MoveX, MoveY, 1.0f, 1.0f, 45.0f);
		RenderMesh(testing, 0.0f, 0.0f, 100, 100, 0.0f);//to delete for testing only
		RenderMesh(pDonutMesh, MoveX, MoveY, 1.0f, 1.0f, 45.0f);


		AEGfxSetCamPosition(MoveX, MoveY);
		// Informing the system about the loop's end
		AESysFrameEnd();
	}
	AEGfxMeshFree(wallMesh);
	AEGfxMeshFree(boundaryMesh);
	AEGfxMeshFree(enemy1);
	free(wallList);
	free(boundaryList);
	AEGfxMeshFree(pDonutMesh);

	// free the system
	AESysExit();
}