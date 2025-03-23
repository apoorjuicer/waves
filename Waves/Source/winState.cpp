/* Start Header ************************************************************************/
/*!
\file		winState.cpp
\author		Henrey Hu
\email		zhihenghenrey.hu@digipen.edu

\brief      This file defines the WinState class, which manages the game's victory screen. 
            It handles rendering the win message, providing options to 
            proceed to the next phase or return to the main menu, and managing state transitions.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "winState.h"

WinState::WinState() : goNext(false), winFont(-1) {}

void WinState::Load() {
    // E.g. set background color or load victory assets
    AEGfxSetBackgroundColor(0.0f, 0.5f, 0.0f); // Dark green background

    winFont = AEGfxCreateFont("Assets/buggy-font.ttf", 80);

}

void WinState::Initialize() {
    goNext = false;
}

void WinState::Update(f32 deltaTime) {
    (f32)deltaTime;

    // Example: Press ENTER to proceed to next level
    if (AEInputCheckTriggered(AEVK_ESCAPE)) {
        goNext = true;
    }

    // Use bool var goNext when player won to trigger currently *So go mainMenu in GetNextState()* as winState only shows when completes all the maps
}

void WinState::Draw() {
    if (winFont != -1) {
        AEGfxPrint(winFont, "You Win", -0.48f, 0.0f, 1.5f, 0.0f, 0.0f, 0.0f, 1.0f);
    }
}

void WinState::Free() {
    // Free any resources if needed
}

void WinState::Unload() {
    // Unload any assets if needed
    if (winFont != -1) {
        AEGfxDestroyFont(winFont);
        winFont = -1;
    }
}

BaseState* WinState::GetNextState() {
    return goNext ? new MainMenuState() : nullptr;
}