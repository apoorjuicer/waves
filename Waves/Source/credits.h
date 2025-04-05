/* Start Header ************************************************************************/
/*!
\file		credits.h
\author		Henrey Hu
\email		zhihenghenrey.hu@digipen.edu

\brief      This file defines the creditState class, which manages the display of game credits.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include "pch.h"
#include "mainMenu.h"

// ========================================
// creditState class for managing game credits
// ========================================
class creditState : public BaseState {
private:
    bool goMenu;                                // Flag for returning to the main menu
	AEGfxTexture* credit = nullptr;             // Texture for the credits
	AEGfxVertexList* credit_mesh = nullptr;     // Mesh for the credits
public:
	creditState();								// Constructor
	void Load() override;						// Load assets
	void Initialize() override;					// Setup initial state data
	void Update(f32 deltaTime) override;		// Handle logic updates
	void Draw() override;						// Render game objects
	void Free() override;						// Free runtime resources
	void Unload() override;						// Unload assets
	BaseState* GetNextState() override;			// Return the next state
};

