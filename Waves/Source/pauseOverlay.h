/* Start Header ************************************************************************/
/*!
\file		pauseOverlay.h
\project    Waves

\author		Henry Hu
\email		zhihenghenrey.hu@digipen.edu

\brief		This file defines the PauseOverlay namespace, which manages the pause overlay in the game.
			It provides functionality for initializing, updating, drawing, and freeing the overlay resources.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include "pch.h"
#include "Player.h"

// Cam position
extern AEVec2 GlobalCenter;

// ======================================================
// PauseOverlay namespace for managing the pause overlay
// ======================================================
namespace PauseOverlay {
	void Init();					// Initialize the overlay
	void Update();					// Update the overlay
	void Draw();					// Draw the overlay
	void Free();					// Free overlay resources
	bool IsPaused();				// Check if the game is paused
	void SetPaused(bool pause);		// Set the pause state
	bool ShouldExitToMenu();		// Check if the game should exit to the menu
}

