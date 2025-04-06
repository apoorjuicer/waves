/* Start Header ************************************************************************/
/*!
\file		gameTransition.cpp
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

#include "gameTransition.h"

// Define constants for button positions and sizes
const f32 kButtonW = 300.0f;
const f32 kButtonH = 100.0f;
const f32 kContinueY = -200.0f;
const f32 kContinueX = -300.0f;
const f32 kQuitY = -200.0f;
const f32 kQuitX = 300.0f;

// Constructor
gameTransition::gameTransition(BaseState* nextState) : continueGame(false), quitGame(false), transitionFont(-1), continueButtonMesh(nullptr), quitButtonMesh(nullptr), nNextState(nextState) {}

// Load assets for the game transition
void gameTransition::Load() {
    AEGfxSetBackgroundColor(0.4f, 0.4f, 0.4f);

    //transitionFont = AEGfxCreateFont("Assets/Fonts/Nunito/Nunito-Bold.ttf", 70);
    transitionFont = FontManager::GetFont("NunitoBold70");

    continueButtonMesh = CreateButtonMesh(0xFF505050);
    quitButtonMesh = CreateButtonMesh(0xFF505050);
}

// Initialize the game transition state
void gameTransition::Initialize() {
    continueGame = false;
    quitGame = false;
}

// Update the game transition state
void gameTransition::Update(f32 deltaTime) {
    (f32)deltaTime;

    if (AEInputCheckTriggered(AEVK_LBUTTON)) {
        if (IsMouseInsideButton(kContinueX, kContinueY, kButtonW, kButtonH)) {
            continueGame = true;
        }
        else if (IsMouseInsideButton(kQuitX, kQuitY, kButtonW, kButtonH)) {
            quitGame = true;
        }
    }
}

// Draw the game transition state
void gameTransition::Draw() 
{
    // --- Draw and center text on the Game Paused Text---
    // The Resume button is drawn at world coordinates: (player.x, player.y + kResumeY)
    // Compute the normalized coordinates for that center point:
    //float winW = (float)AEGfxGetWindowWidth();
    //float winH = (float)AEGfxGetWindowHeight();
    float camX, camY;
    AEGfxGetCamPosition(&camX, &camY);
    float textWidth, textHeight;

    // Get text dimensions (normalized units) for "Game Paused":
    AEGfxGetPrintSize(transitionFont, "CONTINUE TO NEXT LEVEL?", 1.0f, &textWidth, &textHeight);

    // Center the text horizontally by subtracting half its width,
    // and center vertically by subtracting half its height from 0.0f
    // so that it’s in the exact middle of the screen.
    float centerX = -textWidth * 0.5f;
    float centerY = textHeight * 0.5f;

    // Print at scale = 1.0, with white color (1.0f,1.0f,1.0f,1.0f).
    AEGfxPrint(transitionFont, "CONTINUE TO NEXT LEVEL?", centerX, centerY, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    
    // Button center in world space:
    float resumeCenterX = GlobalCenter.x + kContinueX;
    float resumeCenterY = GlobalCenter.y + kContinueY;

    DrawButton(continueButtonMesh, resumeCenterX, resumeCenterY, "YES", transitionFont, kButtonW, kButtonH, 1.0f, 1.0f, 1.0f, 1.0f);

    // --- Draw and center text on the Main Menu button ---
    // Button center in world space:
    float mainMenuCenterX = GlobalCenter.x + kQuitX;
    float mainMenuCenterY = GlobalCenter.y + kQuitY;

    DrawButton(quitButtonMesh, mainMenuCenterX, mainMenuCenterY, "NO", transitionFont, kButtonW, kButtonH, 1.0f, 1.0f, 1.0f, 1.0f);
}

// Free runtime resources
void gameTransition::Free() {
	if (continueButtonMesh) {
		AEGfxMeshFree(continueButtonMesh);
		continueButtonMesh = nullptr;
	}
	if (quitButtonMesh) {
		AEGfxMeshFree(quitButtonMesh);
		quitButtonMesh = nullptr;
	}
}

// Unload assets for the game transition
void gameTransition::Unload() {}

// Return the next state
BaseState* gameTransition::GetNextState() {
    if (continueGame) {
        return nNextState;
    }
    if (quitGame) {
        return new MainMenuState();
    }
    return nullptr;
}