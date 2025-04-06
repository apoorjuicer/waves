/* Start Header ************************************************************************/
/*!
\file		loseState.h
\project    Waves

\author		Henrey Hu
\email		zhihenghenrey.hu@digipen.edu

\brief      This file defines the LoseState class, which manages the game-over screen. 
            It handles rendering the loss message, providing options to return to the main menu, 
            and managing transitions to other game states.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include "pch.h"
#include "mainMenu.h"
#include "Audio.h"
#include "gameTransition.h"

extern AEVec2 GlobalCenter;

class LoseState : public BaseState {
private:
    bool goMenu;                                // Flag for returning to the main menu
	s8 loseFont;                                // Font for the loss message
	s8 buttonFont;                              // Font for the button
	AEGfxVertexList* okButtonMesh = nullptr;    // Mesh for the OK button
public:
	LoseState();                                // Constructor
	void Load() override;                       // Load assets
	void Initialize() override;                 // Initialize data
	void Update(f32 deltaTime) override;		// Update logic
	void Draw() override;						// Render game objects
	void Free() override;						// Free runtime resources
	void Unload() override;						// Unload assets
	BaseState* GetNextState() override;			// Return the next state
};
