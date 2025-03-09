/* Start Header ************************************************************************/
/*!
\file		loseState.cpp
\author		Henrey Hu
\email		zhihenghenrey.hu@digipen.edu

\brief

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "loseState.h"
#include "mainMenu.h"
#include "AEEngine.h"

LoseState::LoseState() : goMenu(false), loseFont(-1) {}

void LoseState::Load() {
    AEGfxSetBackgroundColor(0.5f, 0.0f, 0.0f); // Dark red background

    loseFont = AEGfxCreateFont("Assets/buggy-font.ttf", 80);
}

void LoseState::Initialize() {
    goMenu = false;
}

void LoseState::Update(f32 deltaTime) {
    (f32)deltaTime;

    // Example: Press ENTER to return to main menu
    if (AEInputCheckTriggered(AEVK_RETURN)) {
        goMenu = true;
    }

    // Use bool var goMenu when player 'dies' to trigger go mainMenu in GetNextState()
}

void LoseState::Draw() {
    if (loseFont != -1) {
        AEGfxPrint(loseFont, "Game Over", -0.65f, 0.0f, 1.5f, 0.0f, 0.0f, 0.0f, 1.0f);
    }
}

void LoseState::Free() {}

void LoseState::Unload() {}

BaseState* LoseState::GetNextState() {
    // If goMenu is true, transition to main menu
    return goMenu ? new MainMenuState() : nullptr;
}