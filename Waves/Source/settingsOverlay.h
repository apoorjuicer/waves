/* Start Header ************************************************************************/
/*!
\file		settingsOverlay.h
\author		Henrey Hu
\email		zhihenghenrey.hu@digipen.edu

\brief      This file defines the SettingsOverlay namespace, which manages the settings overlay in the game.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once

#include "pch.h"
#include "Audio.h"

namespace SettingsOverlay {
	void Init();            // Initialize the overlay
	void Update();          // Update the overlay
	void Draw();			// Draw the overlay
	void Free();			// Free overlay resources
    void Toggle();			// Show/hide the overlay
    bool IsVisible();		// Check if currently visible
}

