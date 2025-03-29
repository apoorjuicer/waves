/* Start Header ************************************************************************/
/*!
\file		loseState.cpp
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

#include "loseState.h"

const f32 kButtonW = 300.0f;
const f32 kButtonH = 100.0f;
const f32 kokY = -200.0f;
const f32 kokX = 0.0f;

LoseState::LoseState() : goMenu(false), loseFont(-1), buttonFont(-1), okButtonMesh(nullptr) {}

void LoseState::Load() {
    AEGfxSetBackgroundColor(0.5f, 0.0f, 0.0f); // Dark red background

    loseFont = AEGfxCreateFont("Assets/buggy-font.ttf", 80);
    buttonFont = AEGfxCreateFont("Assets/Fonts/Nunito/Nunito-Bold.ttf", 70);

    okButtonMesh = CreateButtonMesh(0xFF505050);
}

void LoseState::Initialize() {
    goMenu = false;
}

void LoseState::Update(f32 deltaTime) {
    (f32)deltaTime;

    // Example: Press ENTER to return to main menu
    if (AEInputCheckTriggered(AEVK_ESCAPE)) {
        goMenu = true;
    }

    if (AEInputCheckTriggered(AEVK_LBUTTON)) {
        if (IsMouseInsideButton(kokX, kokY, kButtonW, kButtonH)) {
            goMenu = true;
        }
    }
}

void LoseState::Draw() {
    float textWidth, textHeight;
    AEGfxGetPrintSize(loseFont, "GAME OVER", 1.5f, &textWidth, &textHeight);

    float centeredX = -textWidth * 0.5f;
    float centeredY = 0.0f;

    AEGfxPrint(loseFont, "GAME OVER", centeredX, centeredY, 1.5f, 0.0f, 0.0f, 0.0f, 1.0f);

    DrawButton(okButtonMesh, kokX + player.x, kokY + player.y, "OK", buttonFont, kButtonW, kButtonH, 1.0f, 1.0f, 1.0f, 1.0f);
}

void LoseState::Free() {
    if (okButtonMesh) {
        AEGfxMeshFree(okButtonMesh);
        okButtonMesh = nullptr;
    }
}

void LoseState::Unload() {
    if (loseFont != -1) {
        AEGfxDestroyFont(loseFont);
        loseFont = -1;
    }
}

BaseState* LoseState::GetNextState() {
    // If goMenu is true, transition to main menu
    return goMenu ? new MainMenuState() : nullptr;
}