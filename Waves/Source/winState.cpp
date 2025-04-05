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

//const f32 kButtonW = 300.0f;
//const f32 kButtonH = 100.0f;
//const f32 kokY = -200.0f;
//const f32 kokX = 0.0f;

WinState::WinState() : goNext(false), winFont(-1), buttonFont(-1), okButtonMesh(nullptr) {}

void WinState::Load() {
    AEGfxSetCamPosition(0.0f, 0.0f);
    AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f); // Dark green background

    /*winFont = AEGfxCreateFont("Assets/buggy-font.ttf", 80);
    buttonFont = AEGfxCreateFont("Assets/Fonts/Nunito/Nunito-Bold.ttf", 70);*/

    winFont = FontManager::GetFont("Buggy80");
    buttonFont = FontManager::GetFont("NunitoBold70");

    okButtonMesh = CreateButtonMesh(0xFF505050);
}

void WinState::Initialize() {
    goNext = false;
}

void WinState::Update(f32 deltaTime) {
    (f32)deltaTime;

    if (AEInputCheckTriggered(AEVK_ESCAPE)) {
        goNext = true;
    }

    if (AEInputCheckTriggered(AEVK_LBUTTON)) {
        /*if (IsMouseInsideButton(kokX, kokY, kButtonW, kButtonH)) {
            goNext = true;
        }*/

    // ---------------------------
    // "OK" Button
    // ---------------------------
        {
            f32 textW, textH;
            AEGfxGetPrintSize(buttonFont, "OK", 1.0f, &textW, &textH);

            f32 centerX_NDC = 0.0f;
            f32 centerY_NDC = -0.5f;

            if (IsMouseInsideButtonNDC(centerX_NDC, centerY_NDC, textW, textH))
            {
                goNext = true;
            }
        }
    }
}

void WinState::Draw() {
    float textWidth, textHeight;
    AEGfxGetPrintSize(winFont, "YOU WIN", 1.5f, &textWidth, &textHeight);

    float centeredX = -textWidth * 0.5f;
    float centeredY = 0.0f;

    AEGfxPrint(winFont, "YOU WIN", centeredX, centeredY, 1.5f, 0.0f, 1.0f, 0.0f, 1.0f);

    //DrawButton(okButtonMesh, kokX + player.x, kokY + player.y, "OK", buttonFont, kButtonW, kButtonH, 1.0f, 1.0f, 1.0f, 1.0f);

    // ==============================================================
    // Color Change on Hover 'OK' Button
    // ==============================================================    
    {
        f32 ExitTextWidthNDC, ExitTextHeightNDC;
        AEGfxGetPrintSize(buttonFont, "OK", 1.0f, &ExitTextWidthNDC, &ExitTextHeightNDC);

        f32 ExitCenterX_NDC = 0.0f;
        f32 ExitCenterY_NDC = -0.5f;

        bool ExitHovered = IsMouseInsideButtonNDC(ExitCenterX_NDC, ExitCenterY_NDC, ExitTextWidthNDC, ExitTextHeightNDC);
        f32 Exit_r = (ExitHovered) ? 0.0f : 1.0f;
        f32 Exit_g = (ExitHovered) ? 0.5f : 1.0f;
        f32 Exit_b = (ExitHovered) ? 0.0f : 1.0f;

        f32 ExitTextX_NDC = ExitCenterX_NDC - (ExitTextWidthNDC * 0.5f);
        f32 ExitTextY_NDC = ExitCenterY_NDC - (ExitTextHeightNDC * 0.5f);
        AEGfxPrint(buttonFont, "OK", ExitTextX_NDC, ExitTextY_NDC, 1.0f, Exit_r, Exit_g, Exit_b, 1.0f);
    }
}

void WinState::Free() {
    if (okButtonMesh) {
        AEGfxMeshFree(okButtonMesh);
        okButtonMesh = nullptr;
    }
}

void WinState::Unload() {
    // Unload any assets if needed
    /*if (winFont != -1) {
        AEGfxDestroyFont(winFont);
        winFont = -1;
    }*/
}

BaseState* WinState::GetNextState() {
    return goNext ? new MainMenuState() : nullptr;
}