// ---------------------------------------------------------------------------
// includes

#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"
//#include "draw_map.h"
//#include "enemy_henrey.h"
//#include "Player.h"
//#include <cstdlib>

#include <iostream>
#include "sonar.hpp"


// ---------------------------------------------------------------------------
// main

//int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
//	_In_opt_ HINSTANCE hPrevInstance,
//	_In_ LPWSTR    lpCmdLine,
//	_In_ int       nCmdShow)
//{
//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//
//	UNREFERENCED_PARAMETER(hPrevInstance);
//	UNREFERENCED_PARAMETER(lpCmdLine);
//
//
//	int gGameRunning = 1;
//
//	// Initialization of your own variables go here
//
//	// Using custom window procedure
//	AESysInit(hInstance, nCmdShow, 1600, 900, 1, 60, false, NULL);
//
//
//	AEGfxVertexList* wallMesh = createWallMesh(0xFF008080);
//	AEGfxVertexList* boundaryMesh = createBoundaryMesh(0xFF654321);
//	AEGfxVertexList* enemy1 = createEnemyMesh(0xFFFF0000);
//
//	addWall(0.0f, 0.0f, 600.0f, 300.0f);
//	addBoundary(0.0f, 300.0f, 1000.0f, 50.0f);
//	addBoundary(-520.0f, 0.0f, 50.0f, 650.0f);
//	addBoundary(0.0f, -300.0f, 1000.0f, 50.0f);
//	addBoundary(520.0f, 0.0f, 50.0f, 650.0f);
//
//	// Test the walls array
//	for (int i = 0; i < wallCount; i++) {
//		printf("Wall %f: x=%f, y=%f, width=%f, height=%f\n", i, wallList[i].x, wallList[i].y, wallList[i].scale_x, wallList[i].scale_y);
//	}
//
//	// Test the boundaries array
//	for (int i = 0; i < boundaryCount; i++) {
//		printf("Boundary %f: x=%f, y=%f, width=%f, height=%f\n", i, boundaryList[i].x, boundaryList[i].y, boundaryList[i].scale_x, boundaryList[i].scale_y);
//	}
//	AEGfxVertexList* pDonutMesh = CreateDonutMesh(1000.0f, 200.0f, 48, 0x00000000);
//	AEGfxVertexList* PlayerMesh = Player_Mesh(48, 0x00000000, 10.0f);
//	AEGfxVertexList* testing = testing_only(); //to delete for testing only
//
//	// Changing the window title
//	AESysSetWindowTitle("My New Demo!");
//
//	// reset the system modules
//	AESysReset();
//
//	//printf("Hello World\n");
//	f32 MoveX = 0.0f;
//	f32 MoveY = 0.0f;
//	f32 dt = (f32)AEFrameRateControllerGetFrameTime();
//
//	// Game Loop
//	while (gGameRunning)
//	{
//		// Informing the system about the loop's start
//		AESysFrameStart();
//		AEGfxSetBackgroundColor(0.5, 0.5, 0.5);
//		// Basic way to trigger exiting the application
//		// when ESCAPE is hit or when the window is closed
//		if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
//			gGameRunning = 0;
//
//		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
//		// Your own update logic goes here
//		//player movement
//		if (AEInputCheckCurr(AEVK_W))
//		{
//			MoveY += 1000*dt;
//		}
//		if (AEInputCheckCurr(AEVK_S))
//		{
//			MoveY -= 1000 * dt;
//		}
//		if (AEInputCheckCurr(AEVK_A))
//		{
//			MoveX -= 1000 * dt;
//		}
//		if (AEInputCheckCurr(AEVK_D))
//		{
//			MoveX += 1000 * dt;
//		}
//
//		// Your own rendering logic goes here
//		drawWalls(wallMesh);
//		drawBoundaries(boundaryMesh);
//		RenderMesh(enemy1, 450.0f, -200.0f, 50.0f, 50.0f, 0.0f);
//		//drawing the donut mesh
//		RenderMesh(PlayerMesh, MoveX, MoveY, 1.0f, 1.0f, 45.0f);
//		RenderMesh(testing, 0.0f, 0.0f, 100, 100, 0.0f);//to delete for testing only
//		RenderMesh(pDonutMesh, MoveX, MoveY, 1.0f, 1.0f, 45.0f);
//
//
//		AEGfxSetCamPosition(MoveX, MoveY);
//		// Informing the system about the loop's end
//		AESysFrameEnd();
//	}
//	AEGfxMeshFree(wallMesh);
//	AEGfxMeshFree(boundaryMesh);
//	AEGfxMeshFree(enemy1);
//	free(wallList);
//	free(boundaryList);
//	AEGfxMeshFree(pDonutMesh);
//
//	// free the system
//	AESysExit();
//}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);


	int gGameRunning = 1;

	// Using custom window procedure
	AESysInit(hInstance, nCmdShow, 1600, 900, 1, 60, false, NULL);

	// Changing the window title
	AESysSetWindowTitle("My New Demo!");

	// reset the system modules
	AESysReset();

	//=============================================================================
	AEGfxVertexList* donutmesh = CreateDonutMesh(1.0f, 0.95f, 36);
	AEGfxVertexList* semicirclemesh = Create_SemiCircle_Mesh(1.0f, 0.95f, 36);

	wave_s wave[3] = {};
	// testing waves;
	wave[0] = { -400.f, 50.f, 1.f, 150.f, true, false, 50.f, donutmesh };
	wave[1] = { 350.f, 100.f, 1.f, 150.f, true, false, 50.f, donutmesh };
	wave[2] = { 50.f, 50.f, 1.f, 150.f, true, false, 50.f, donutmesh };

	//=============================================================================
	// initialize reflect waves;
	wave_s reflection_waves[MAX_NUM_REFLECTIONS];
	init_reflection_waves(reflection_waves, semicirclemesh);
	//=============================================================================


	AEGfxMeshStart();

	AEGfxTriAdd(
		-0.5f, -0.5f, 0x00000000, 0.0f, 1.0f,
		0.5f, -0.5f, 0x00000000, 1.0f, 1.0f,
		-0.5f, 0.5f, 0x00000000, 0.0f, 0.0f);

	AEGfxTriAdd(
		0.5f, -0.5f, 0x00000000, 1.0f, 1.0f,
		0.5f, 0.5f, 0x00000000, 1.0f, 0.0f,
		-0.5f, 0.5f, 0x00000000, 0.0f, 0.0f);
	
	AEGfxVertexList* squaremesh = AEGfxMeshEnd();

	wall_s walls[2] = {};
	walls[0] = { 200.f, 50.f, 100.f, 400.f, false };
	walls[1] = { 350.f, 250.f, 500.f, 100.f, false };

	int count{1};
	//=============================================================================

	
	// Game Loop
	while (gGameRunning)
	{
		// Informing the system about the loop's start
		AESysFrameStart();
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetBackgroundColor(0.5, 0.5, 0.5);

		// Exit application
		if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
			gGameRunning = 0;

		// for testing;
		if (AEInputCheckTriggered(AEVK_LBUTTON)) {
			//AEInputGetCursorPosition(&wave2.x, &wave2.y);
			//wave[0].alive = true;
			//wave[1].alive = true;
			wave[2].alive = true;
		}
		if (AEInputCheckTriggered(AEVK_RBUTTON)) {
			wave[1].alive = true;
		}

		//=============================================================================
		// update size of main waves;
		for (wave_s& i : wave) {
			// skip wave if not alive;
			if (i.alive) {
				update_waves(i);

				// check for collision with any walls for current live wave;
				for (wall_s const& w : walls) {
					// if colliding; spawn reflection wave;
					if (wave_collision(i, w) && !w.reflected) {
						spawn_reflection_wave(i, w, reflection_waves);
					}
				}
			}
		}

		// update size of reflection waves;
		for (wave_s& i : reflection_waves) {
			// skip wave if not alive;
			if (i.alive) {
				update_waves(i);
			}
		}
		/*=============================================================================		
		 for testing;
		if (wave_collision(wave[1], wall1)) {
			std::cout << "COLLISION DETECTED\n";
			count = 1;
		}
		else if (1 == count) {
			std::cout << "NO COLLISION\n";
			count = 0;
		}*/
		
		//=============================================================================		
		// draw main waves;
		for (wave_s& i : wave) {
			if (i.alive) {
				draw_wave(i, donutmesh);
			}
		}
		// draw reflection waves;
		for (wave_s& i : reflection_waves) {
			if (i.alive) {
				draw_wave(i, semicirclemesh);
			}
		}

		// draw walls;
		for (wall_s const& i : walls) {
			draw_wall(i, squaremesh);
		}
		

		// AEGfxSetCamPosition(MoveX, MoveY);
		// Informing the system about the loop's end
		AESysFrameEnd();
	}
	AEGfxMeshFree(donutmesh);
	AEGfxMeshFree(squaremesh);
	AEGfxMeshFree(semicirclemesh);

	// free the system
	AESysExit();
}