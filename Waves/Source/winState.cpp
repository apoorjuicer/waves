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

const f32 kButtonW = 300.0f;
const f32 kButtonH = 100.0f;
const f32 kokY = -200.0f;
const f32 kokX = 0.0f;

WinState::WinState() : goNext(false), winFont(-1), buttonFont(-1), okButtonMesh(nullptr) {}

void WinState::Load() {
    AEGfxSetBackgroundColor(0.0f, 0.5f, 0.0f); // Dark green background

    winFont = AEGfxCreateFont("Assets/buggy-font.ttf", 80);
    buttonFont = AEGfxCreateFont("Assets/Fonts/Nunito/Nunito-Bold.ttf", 70);

    okButtonMesh = CreateButtonMesh(0xFF505050);
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

    if (AEInputCheckTriggered(AEVK_LBUTTON)) {
        if (IsMouseInsideButton(kokX, kokY, kButtonW, kButtonH)) {
            goNext = true;
        }
    }
}

void WinState::Draw() {
    if (winFont != -1) {
        AEGfxPrint(winFont, "YOU WIN", -0.48f, 0.0f, 1.5f, 0.0f, 0.0f, 0.0f, 1.0f);
    }

    DrawButton(okButtonMesh, kokX + player.x, kokY + player.y, "OK", buttonFont, kButtonW, kButtonH, 1.0f, 1.0f, 1.0f, 1.0f);
}

void WinState::Free() {
    if (okButtonMesh) {
        AEGfxMeshFree(okButtonMesh);
        okButtonMesh = nullptr;
    }
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