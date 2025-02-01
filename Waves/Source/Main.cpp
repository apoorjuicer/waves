// ---------------------------------------------------------------------------
// includes

#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"
//#include "draw_map.h"
//#include "enemy_henrey.h"
//#include "Player.h"
//#include <cstdlib>
#include "Player.h"
#include "Maze.h"
#include "Rock.h"

#include "enemy_henrey.h"
#include <iostream>
#include "sonar.hpp"


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
	f32 tilesize = 100.0f;

	// Using custom window procedure
	AESysInit(hInstance, nCmdShow, 1600, 900, 1, 60, false, NULL);


	AEGfxVertexList* pDonutMesh = CreateDonutMesh(1000.0f, 100.0f, 48, 0x00000000);
	AEGfxVertexList* PlayerMesh = Player_Mesh(48, 0xFFFFFFFF, 10.0f);

	//change first parameter for tile size

	AEGfxVertexList* wallMesh = CreateSquareMesh(tilesize, 0xFF000000); // Blue for walls
	AEGfxVertexList* pathMesh = CreateSquareMesh(tilesize, 0x55555555); // White for paths
	AEGfxVertexList* enemy1 = createEnemyMesh(0xFFFF0000); //draw enemy

	// Changing the window title
	AESysSetWindowTitle("My New Demo!");

	// reset the system modules
	AESysReset();

	//=============================================================================
	AEGfxVertexList* wavemesh = CreateWaveMesh(1.0f, 0.95f, 36);
	AEGfxVertexList* semicirclemesh = Create_SemiCircle_Mesh(1.0f, 0.98f, 36, 2);
	AEGfxVertexList* linemesh_Origin = Create_LineMesh(false);
	AEGfxVertexList* linemesh_Skewed = Create_LineMesh(true);
	// array of wave meshes;
	AEGfxVertexList* wave_meshes[NUM_WAVE_MESHES] = { wavemesh, semicirclemesh, linemesh_Origin, linemesh_Skewed };
	// array of waves;
	wave_s wave[MAX_NUM_WAVES] = {};

	//=============================================================================
	// initialize reflect waves;
	wave_s reflection_waves[MAX_NUM_WAVES];
	init_reflection_waves(reflection_waves, semicirclemesh);
	//=============================================================================
	// outlines;
	line_s outlines[MAX_NUM_OUTLINES] = {};

	//=============================================================================

	
	// Game Loop
	while (gGameRunning)
	{
		// Informing the system about the loop's start
		AESysFrameStart();
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetBackgroundColor(0.f, 0.f, 0.f);

		// Exit application
		if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
			gGameRunning = 0;

		// =============================================================================
		// update size of main waves;
		for (wave_s& i : wave) {
			// skip wave if not alive;
			if (i.alive) {
		 		update_waves(i);
				// spawn reflection waves on collision with walls
				reflections_spawner(i, maze, tilesize, reflection_waves, outlines, wave_meshes);
			}
		}
		// update reflection waves;
		for (wave_s& i : reflection_waves) {
			// skip wave if not alive;
			if (i.alive) {
				update_waves(i);
			}
		}
		// update outlines;
		for (line_s& l : outlines) {
			if (l.alive) {
				update_outlines(l);
			}
		}

		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		//player movement
		float speed = 120.0f; // Speed in units per second
		float dt = (float)AEFrameRateControllerGetFrameTime();

		if (AEInputCheckCurr(AEVK_W)) MovePlayer(player, 0.0f, 1.0f, speed, dt, maze, tilesize); // Move up
		if (AEInputCheckCurr(AEVK_S)) MovePlayer(player, 0.0f, -1.0f, speed, dt, maze, tilesize);  // Move down
		if (AEInputCheckCurr(AEVK_A)) MovePlayer(player, -1.0f, 0.0f, speed, dt, maze, tilesize); // Move left
		if (AEInputCheckCurr(AEVK_D)) MovePlayer(player, 1.0f, 0.0f, speed, dt, maze, tilesize);  // Move right

		// spawn wave on player; for testing;
		if (AEInputCheckTriggered(AEVK_SPACE)) {
			spawn_wave(wave, player.x, player.y, wavemesh);
		}

		// Your own rendering logic goes here
		RenderPlayer(player, PlayerMesh);
		rock::drawRock(player, maze, tilesize, 0.0f, 0.0f, wave, wavemesh);
		RenderMesh(PlayerMesh, player.x, player.y, 1.0f, 1.0f, 45.0f);
		RenderMesh(enemy1, 350.0f, -100.0f, 50.0f, 50.0f, 0.0f);
		//RenderMesh(pDonutMesh, player.x, player.y, 1.0f, 1.0f, 45.0f);

		draw_wave(wave);
		draw_wave(reflection_waves);
		// draw outlines;
		for (line_s& l : outlines) {
			if (l.alive) {
				draw_outlines(l, wave_meshes);
			}
		}
		RenderMaze(maze, tilesize, wallMesh, pathMesh); // 100.0f is the tile size

		// AEGfxSetCamPosition(MoveX, MoveY);
		AEGfxSetCamPosition(player.x, player.y);
		// Informing the system about the loop's end
		AESysFrameEnd();
	}
	// free all meshes for waves and outlines;
	for (AEGfxVertexList* (&i) : wave_meshes) {
		if (!i) {
			continue;
		}
		AEGfxMeshFree(i);
	}

	// free the system
	AESysExit();
}