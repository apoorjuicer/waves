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


	AEGfxVertexList* pDonutMesh = CreateDonutMesh(500.0f, 100.0f, 36, 0xFFFF0000);

	// Changing the window title
	AESysSetWindowTitle("My New Demo!");

	// reset the system modules
	AESysReset();

	printf("Hello World\n");

	// Game Loop
	while (gGameRunning)
	{
		// Informing the system about the loop's start
		AESysFrameStart();
		AEGfxSetBackgroundColor(0.0, 0.0, 0.0);
		// Basic way to trigger exiting the application
		// when ESCAPE is hit or when the window is closed
		if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
			gGameRunning = 0;

		// Your own update logic goes here
		RenderMesh(pDonutMesh, 0.0f, 0.0f, 1.0f, 1.0f, 45.0f); 

		// Your own rendering logic goes here


		// Informing the system about the loop's end
		AESysFrameEnd();
	}
	AEGfxMeshFree(pDonutMesh);

	// free the system
	AESysExit();
}