/* Start Header ************************************************************************/
/*!
\file		gamePreview.h
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
#include "Player.h"

// ========================================
// gamePreview class for managing the game preview state
// ========================================
class gamePreview : public BaseState {
private:
    bool goGame = false;						// Flag for returning to the main menu
	bool back2Main = false;						// Flag for returning to the main menu
	s8 previewFont;								// Font for the preview
	AEGfxVertexList* maze1_mesh = nullptr;		// Mesh for the maze preview
	int level_num;								// Level number for the preview
public:
	gamePreview(int i);							// Constructor
	void Load() override;						// Load assets
	void Initialize() override;					// Initialize data
	void Update(f32 deltaTime) override;		// Update logic
	void Draw() override;						// Render game objects
	void Free() override;						// Free runtime resources
	void Unload() override;						// Unload assets
	BaseState* GetNextState() override;			// Return the next state
};
