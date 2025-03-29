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

	// Shutdown and free fonts
	FontManager::ShutdownFonts();

	// free the system
	AESysExit();
}