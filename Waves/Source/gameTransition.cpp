/* Start Header ************************************************************************/
/*!
\file		gameTransition.cpp
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
#include "mainMenu.h"
#include "game1.h"
#include "game2.h"
#include "AEEngine.h"
#include "Player.h"

const f32 kButtonW = 300.0f;
const f32 kButtonH = 100.0f;
const f32 kContinueY = -200.0f;
const f32 kContinueX = -300.0f;
const f32 kQuitY = -200.0f;
const f32 kQuitX = 300.0f;

gameTransition::gameTransition() : continueGame(false), quitGame(false), transitionFont(-1), continueButtonMesh(nullptr), quitButtonMesh(nullptr) {}

void gameTransition::Load() {
    AEGfxSetBackgroundColor(0.f, 0.f, 0.f);

    transitionFont = AEGfxCreateFont("Assets/liberation-mono.ttf", 70);

    continueButtonMesh = CreateButtonMesh(kButtonW, kButtonH, 0xFF202020);
    quitButtonMesh = CreateButtonMesh(kButtonW, kButtonH, 0xFF202020);
}

void gameTransition::Initialize() {
    AEInputReset();    // Reset any prior input events
    continueGame = false;
    quitGame = false;
}

void gameTransition::Update(f32 deltaTime) {
    (f32)deltaTime;

    //// Example: Press ENTER to return to main menu
    //if (AEInputCheckTriggered(AEVK_RETURN)) {
    //    continueGame = true;
    //}

    //if (AEInputCheckTriggered(AEVK_LBUTTON)) {
    //    if (IsMouseInsideButton(0.0f, -300.0f, 200.0f, 100.0f)) {
    //        continueGame = true;
    //    }
    //}

    if (AEInputCheckTriggered(AEVK_LBUTTON)) {
        if (IsMouseInsideButton(kContinueX, kContinueY, kButtonW, kButtonH)) {
            continueGame = true;
        }
        else if (IsMouseInsideButton(kQuitX, kQuitY, kButtonW, kButtonH)) {
            quitGame = true;
        }
    }
}

void gameTransition::Draw() {

    AEMtx33 t;

    // --- Draw and center text on the Game Paused Text---
    // The Resume button is drawn at world coordinates: (player.x, player.y + kResumeY)
    // Compute the normalized coordinates for that center point:
    float winW = (float)AEGfxGetWindowWidth();
    float winH = (float)AEGfxGetWindowHeight();
    float camX, camY;
    AEGfxGetCamPosition(&camX, &camY);
    float textWidth, textHeight;

    // Get text dimensions (normalized units) for "Game Paused":
    AEGfxGetPrintSize(transitionFont, "Continue", 1.0f, &textWidth, &textHeight);

    // Center the text horizontally by subtracting half its width,
    // and center vertically by subtracting half its height from 0.0f
    // so that it’s in the exact middle of the screen.
    float centerX = -textWidth * 0.5f;
    float centerY = textHeight * 0.5f;

    // Print at scale = 1.0, with white color (1.0f,1.0f,1.0f,1.0f).
    AEGfxPrint(transitionFont, "Continue?", centerX, centerY, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    
    // Button center in world space:
    float resumeCenterX = player.x + kContinueX;
    float resumeCenterY = player.y + kContinueY;

    // Convert to normalized screen coordinates:
    float normX = (resumeCenterX - camX) / (winW / 2.0f);
    float normY = (resumeCenterY - camY) / (winH / 2.0f);

    // Get text dimensions (normalized units) for "Resume":
    AEGfxGetPrintSize(transitionFont, "Yes", 1.0f, &textWidth, &textHeight);

    // Offset so that the text's center aligns with the button center:
    normX -= textWidth / 2.0f;
    normY -= textHeight / 2.0f;

    // Draw the Resume button mesh:
    AEMtx33Trans(&t, player.x + kContinueX, player.y + kContinueY);
    AEGfxSetTransform(t.m);
    AEGfxMeshDraw(continueButtonMesh, AE_GFX_MDM_TRIANGLES);

    // Print the centered text:
    AEGfxPrint(transitionFont, "Yes", normX, normY, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    // --- Draw and center text on the Main Menu button ---
    // Button center in world space:
    float mainMenuCenterX = player.x + kQuitX;
    float mainMenuCenterY = player.y + kQuitY;

    // Convert to normalized coordinates:
    normX = (mainMenuCenterX - camX) / (winW / 2.0f);
    normY = (mainMenuCenterY - camY) / (winH / 2.0f);

    // Get text dimensions for "Main Menu":
    AEGfxGetPrintSize(transitionFont, "NO", 1.0f, &textWidth, &textHeight);

    // Offset to center the text:
    normX -= textWidth / 2.0f;
    normY -= textHeight / 2.0f;

    // Draw the Main Menu button mesh:
    AEMtx33Trans(&t, player.x + kQuitX, player.y + kQuitY);
    AEGfxSetTransform(t.m);
    AEGfxMeshDraw(quitButtonMesh, AE_GFX_MDM_TRIANGLES);

    // Print the centered text:
    AEGfxPrint(transitionFont, "NO", normX, normY, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    // Switch to color render mode
    //AEGfxSetRenderMode(AE_GFX_RM_COLOR);
}

void gameTransition::Free() {
}

void gameTransition::Unload() {}

BaseState* gameTransition::GetNextState() {
    if (continueGame) {
        return new GameState2();
    }
    if (quitGame) {
        return new MainMenuState();
    }
    return nullptr;
}