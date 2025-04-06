/* Start Header ************************************************************************/
/*!
\file		instructions.h
\project    Waves

\author		Henrey Hu
\email		zhihenghenrey.hu@digipen.edu

\brief      This file defines the instructionState class, which manages the display of game instructions. 
            It handles loading, rendering, and user interactions, 
            allowing players to view gameplay guidelines before returning to the main menu.

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
// instructionState class for managing game instructions
// ========================================
class instructionState : public BaseState {
private:
    bool goMenu;                            // Flag for returning to the main menu
	AEGfxTexture* guide = nullptr;          // Texture for the instructions
	AEGfxVertexList* guide_mesh = nullptr;  // Mesh for the instructions
public:
	instructionState();                     // Constructor
	void Load() override;                   // Load assets
	void Initialize() override;             // Initialize data
	void Update(f32 deltaTime) override;    // Update logic
	void Draw() override;					// Render game objects
	void Free() override;					// Free runtime resources
	void Unload() override;					// Unload assets
	BaseState* GetNextState() override;		// Return the next state
};
