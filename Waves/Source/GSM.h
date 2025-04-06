/* Start Header ************************************************************************/
/*!
\file		GSM.h
\project    Waves

\author		Henrey Hu
\email		zhihenghenrey.hu@digipen.edu

\brief      This file defines the GameStateManager class, 
            which handles transitions between different game states. 
            It manages the current state, updates the game loop, and ensures 
            proper rendering and cleanup of states, maintaining smooth state transitions.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once

#include "pch.h"

// ========================================
// GameStateManager class for managing game states
// ========================================
class GameStateManager {
private:
	BaseState* currentState = nullptr;							// Pointer to the current game state
	int* pGameRunning = nullptr;								// Pointer to the game running flag
public:
	GameStateManager();											// Constructor

	void SetGameRunningPtr(int* ptr) { pGameRunning = ptr; }	// Set the game running pointer

	void SetState(BaseState* newState);							// Set the current game state
	void Update(f32 deltaTime);									// Update the current state
	void Draw();												// Draw the current state
	~GameStateManager();										// Destructor
};

