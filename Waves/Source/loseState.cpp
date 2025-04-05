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

// Constructor
LoseState::LoseState() : goMenu(false), loseFont(-1), buttonFont(-1), okButtonMesh(nullptr) {}

// Load assets for the lose state
void LoseState::Load() {
    AEGfxSetCamPosition(0.0f, 0.0f);

    AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f); 

    loseFont = FontManager::GetFont("Buggy80");
    buttonFont = FontManager::GetFont("NunitoBold70");

    okButtonMesh = CreateButtonMesh(0xFF505050);
}

// Initialize the lose state
void LoseState::Initialize() {
    goMenu = false;
}

// Update the lose state
void LoseState::Update(f32 deltaTime) {
    (f32)deltaTime;

    if (AEInputCheckTriggered(AEVK_ESCAPE)) {
        goMenu = true;
    }

    if (AEInputCheckTriggered(AEVK_LBUTTON)) {
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
                goMenu = true;
            }
        }
    }
}

// Draw the lose state
void LoseState::Draw() {
    float textWidth, textHeight;
    AEGfxGetPrintSize(loseFont, "GAME OVER", 1.5f, &textWidth, &textHeight);

    float centeredX = -textWidth * 0.5f;
    float centeredY = 0.0f;

    AEGfxPrint(loseFont, "GAME OVER", centeredX, centeredY, 1.5f, 1.0f, 0.0f, 0.0f, 1.0f);

    // ==============================================================
    // Color Change on Hover 'OK' Button
    // ==============================================================    
    {
        f32 ExitTextWidthNDC, ExitTextHeightNDC;
        AEGfxGetPrintSize(buttonFont, "OK", 1.0f, &ExitTextWidthNDC, &ExitTextHeightNDC);

        f32 ExitCenterX_NDC = 0.0f;
        f32 ExitCenterY_NDC = -0.5f;

        bool ExitHovered = IsMouseInsideButtonNDC(ExitCenterX_NDC, ExitCenterY_NDC, ExitTextWidthNDC, ExitTextHeightNDC);
        f32 Exit_r = (ExitHovered) ? 0.5f : 1.0f;
        f32 Exit_g = (ExitHovered) ? 0.0f : 1.0f;
        f32 Exit_b = (ExitHovered) ? 0.0f : 1.0f;

        f32 ExitTextX_NDC = ExitCenterX_NDC - (ExitTextWidthNDC * 0.5f);
        f32 ExitTextY_NDC = ExitCenterY_NDC - (ExitTextHeightNDC * 0.5f);
        AEGfxPrint(buttonFont, "OK", ExitTextX_NDC, ExitTextY_NDC, 1.0f, Exit_r, Exit_g, Exit_b, 1.0f);
    }
}

// Free runtime resources
void LoseState::Free() {
    if (okButtonMesh) {
        AEGfxMeshFree(okButtonMesh);
        okButtonMesh = nullptr;
    }
}

// Unload assets for the lose state
void LoseState::Unload() {}

// Return the next state
BaseState* LoseState::GetNextState() {
    return goMenu ? new MainMenuState() : nullptr;
}