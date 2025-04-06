/* Start Header ************************************************************************/
/*!
\file		gameTransition.h
\project    Waves

\author		Henrey Hu
\email		zhihenghenrey.hu@digipen.edu

\brief      This file defines the gamePreview class, which represents a preview state before entering the main game.
            It manages the loading, rendering, and interactions within the preview,
            including displaying a maze preview and an "OK" button to proceed.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include "pch.h"
#include "mainMenu.h"
#include "game1.h"
#include "game2.h"
#include "Player.h"

// Cam position
extern AEVec2 GlobalCenter;

// ========================================================
// gameTransition class for managing the game transition state
// ========================================================
class gameTransition : public BaseState {
private:
    bool continueGame = false;                      // Flag for going to next Game
    bool quitGame = false;                          // Flag for returning to the main menu
	s8 transitionFont; 		                        // Font for the transition
	AEGfxVertexList* continueButtonMesh = nullptr;  // Mesh for continue button
	AEGfxVertexList* quitButtonMesh = nullptr;      // Mesh for quit button
    BaseState* nNextState;                          // Pointer to the next state 
public:
	gameTransition(BaseState* nextState);			// Constructor
	void Load() override;							// Load assets
	void Initialize() override;						// Initialize data
	void Update(f32 deltaTime) override;			// Update logic
	void Draw() override;							// Render game objects
	void Free() override;							// Free runtime resources
	void Unload() override;							// Unload assets
	BaseState* GetNextState() override;				// Return the next state
};
