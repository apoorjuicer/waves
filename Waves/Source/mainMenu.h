/* Start Header ************************************************************************/
/*!
\file		mainMenu.h
\author		Henrey Hu
\email		zhihenghenrey.hu@digipen.edu

\brief      This file defines the MainMenuState class, which manages the game's main menu. 
            It handles user interactions, such as starting the game, accessing the guide, or exiting. 
            The file also includes utility functions for detecting mouse interactions with buttons and creating button meshes.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include "pch.h"
#include "game1.h"
#include "select.h"
#include "gamePreview.h"
#include "instructions.h"
#include "Audio.h"
#include "Player.h"
#include "settingsOverlay.h"
#include "credits.h"
#include "exitOverlay.h"

// ======================================================
// Internal variables for the main menu
// ======================================================
class MainMenuState : public BaseState {
private:
	bool startGame = false;                                 // Flag for starting the game
	bool guide = false; 	                                // Flag for showing the guide
	bool credit = false;                                    // Flag for showing the credits
	bool setting = false;                                   // Flag for showing the settings
    int* pGameRunning;                                      // <-- Pointer to gGameRunning
	s8 menuFont;                                            // Font for the menu
	s8 buttonFont;                                          // Font for the buttons
    AEGfxVertexList* playButtonMesh = nullptr;              // Mesh for the 'Play' button
    AEGfxVertexList* guideButtonMesh = nullptr;             // Mesh for 'Guide' button
    AEGfxVertexList* creditButtonMesh = nullptr;            // Mesh for 'Credits' button
    AEGfxVertexList* settingButtonMesh = nullptr;           // Mesh for 'Settings' button
    AEGfxVertexList* exitButtonMesh = nullptr;              // Mesh for 'Exit' button
	bool waitingForMouseRelease = true; 		            // Prevents multiple clicks  

public:
	MainMenuState();                                            // Constructor

    // Let the caller inject a pointer to gGameRunning
    void SetGameRunningPtr(int* ptr) { pGameRunning = ptr; }

	void Load() override;                                       // Load assets
	void Initialize() override;                                 // Initialize data
	void Update(f32 deltaTime) override;                        // Update logic
	void Draw() override;                                       // Render game objects
	void Free() override;										// Free runtime resources
	void Unload() override;										// Unload assets
	BaseState* GetNextState() override;							// Return the next state	
};
