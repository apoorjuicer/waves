/* Start Header ************************************************************************/
/*!
\file		instructions.cpp
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

#include "instructions.h"
#include "mainMenu.h"
#include "AEEngine.h"

instructionState::instructionState() : goMenu(false){}

void instructionState::Load() {
    AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

}

void instructionState::Initialize() {
    goMenu = false;
}

void instructionState::Update(f32 deltaTime) {
    (f32)deltaTime;

    // Example: Press ENTER to return to main menu
    if (AEInputCheckTriggered(AEVK_RETURN)) {
        goMenu = true;
    }
}

void instructionState::Draw() {}

void instructionState::Free() {}

void instructionState::Unload() {}

BaseState* instructionState::GetNextState() {
    // If goMenu is true, transition to main menu
    return goMenu ? new MainMenuState() : nullptr;
}