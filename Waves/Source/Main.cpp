// ---------------------------------------------------------------------------
// includes

#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"
#include "Player.h"



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


	AEGfxVertexList* pDonutMesh = CreateDonutMesh(1000.0f, 100.0f, 48, 0x00000000);

	// Changing the window title
	AESysSetWindowTitle("My New Demo!");

	// reset the system modules
	AESysReset();

	printf("Hello World\n");
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
		RenderMesh(pDonutMesh, MoveX, MoveY, 1.0f, 1.0f, 45.0f);

		// Informing the system about the loop's end
		AESysFrameEnd();
	}
	AEGfxMeshFree(pDonutMesh);

	// free the system
	AESysExit();
}