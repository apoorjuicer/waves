/* Start Header ************************************************************************/
/*!
\file		exitOverlay.h
\project    Waves

\author		Henrey Hu
\email		zhihenghenrey.hu@digipen.edu

\brief      This file defines the ExitConfirmOverlay namespace, which manages the exit confirmation overlay in the game.
			It provides functionality for initializing, updating, drawing, and freeing the overlay resources.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once

#include "pch.h"

// ========================================
// ExitConfirmOverlay namespace for managing exit confirmation overlay
// ========================================
namespace ExitConfirmOverlay {
	void Init();						// Initialize the overlay
	void Update(int* pGameRunning);		// Update the overlay
	void Draw();						// Draw the overlay
	void Free();						// Free overlay resources

	void Toggle();						// Toggle the overlay visibility
	bool IsVisible();					// Check if the overlay is visible
}

