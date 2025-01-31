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
	wave[1] = { 350.f, 80.f, 1.f, 150.f, true, false, 50.f, donutmesh };
	wave[2] = { 50.f, 100.f, 1.f, 150.f, true, false, 50.f, donutmesh };

	//=============================================================================
	// initialize reflect waves;
	wave_s reflection_waves[MAX_NUM_REFLECTIONS];
	init_reflection_waves(reflection_waves, semicirclemesh);
	//=============================================================================
	// outlines;
	line_s outlines[MAX_NUM_OUTLINES] = {};



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

	wall_s walls[3] = {};
	walls[0] = { 200.f, 50.f, 100.f, 400.f, false };
	walls[1] = { 350.f, -80.f, 500.f, 100.f, false };
	walls[2] = { -100.f, 50.f, 80.f, 600.f, false };

	int count{1};
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

		// for testing;
		if (AEInputCheckTriggered(AEVK_LBUTTON)) {
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
					if (wave_collision(i, w)) {
						spawn_reflection_wave(i, w, reflection_waves);
						spawn_outlines(i, w, outlines);
					}
				}
			}
		}

		for (line_s& l : outlines) {
			if (l.alive) {
				update_outlines(l);
			}
		}

		// update size of reflection waves;
		for (wave_s& i : reflection_waves) {
			// skip wave if not alive;
			if (i.alive) {
				update_waves(i);
			}
		}
		//=============================================================================		

		//=============================================================================		
		AEGfxSetColorToAdd(0.f, 0.f, 0.f, 0.f);
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
		AEGfxSetColorToAdd(1.f, 1.f, 1.f, 1.f);
		// draw outlines;
		for (line_s& l : outlines) {
			if (l.alive) {
				draw_outlines(l, squaremesh);
			}
		}
		AEGfxSetColorToAdd(0.f, 0.f, 0.f, 0.f);
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