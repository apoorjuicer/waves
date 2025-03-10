/* Start Header ************************************************************************/
/*!
\file		pauseOverlay.cpp
\author		Henry Hu
\email		zhihenghenrey.hu@digipen.edu
\brief      

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "pauseOverlay.h"
#include "mainMenu.h"
#include "Player.h"

// Internal variables (file scope)
namespace {
    bool sPaused = false;
    bool sExitToMenu = false;
    s8 sFont = -1;
    s8 mFont = -1;
    AEGfxVertexList* pauseScreenMesh = nullptr;
    AEGfxVertexList* sResumeMesh = nullptr;
    AEGfxVertexList* sMainMenuMesh = nullptr;

    const f32 kButtonW = 300.0f;
    const f32 kButtonH = 100.0f;
    const f32 kResumeY = -200.0f;
    const f32 kResumeX = -300.0f;
    const f32 kMainMenuY = -200.0f;
    const f32 kMainMenuX = 300.0f;
    const f32 pauseMenuX = 1000.0f;
    const f32 pauseMenuY = 700.0f;
}

namespace PauseOverlay {
    void Init() {
        sPaused = false;
        sExitToMenu = false;
        sFont = AEGfxCreateFont("Assets/liberation-mono.ttf", 50);
        mFont = AEGfxCreateFont("Assets/liberation-mono.ttf", 100);
        pauseScreenMesh = CreateButtonMesh(pauseMenuX, pauseMenuY, 0xFF404040);
        sResumeMesh = CreateButtonMesh(kButtonW, kButtonH, 0xFF202020);
        sMainMenuMesh = CreateButtonMesh(kButtonW, kButtonH, 0xFF202020);
    }

    void Update() {
        if (!sPaused) return;
        if (AEInputCheckTriggered(AEVK_LBUTTON)) {
            if (IsMouseInsideButton(kResumeX, kResumeY, kButtonW, kButtonH)) {
                sPaused = false;
                AEInputReset();
            }
            else if (IsMouseInsideButton(kMainMenuX, kMainMenuY, kButtonW, kButtonH)) {
                sExitToMenu = true;
                sPaused = false;
                AEInputReset();
            }
        }
    }

    void Draw() {
        if (!sPaused) return;

        AEMtx33 t;

        // Draw pause screen background (using player's world position as base)
        AEMtx33Trans(&t, player.x, player.y);
        AEGfxSetTransform(t.m);
        AEGfxMeshDraw(pauseScreenMesh, AE_GFX_MDM_TRIANGLES);

        // --- Draw and center text on the Resume button ---
        // The Resume button is drawn at world coordinates: (player.x, player.y + kResumeY)
        // Compute the normalized coordinates for that center point:
        float winW = (float)AEGfxGetWindowWidth();
        float winH = (float)AEGfxGetWindowHeight();
        float camX, camY;
        AEGfxGetCamPosition(&camX, &camY);
        float textWidth, textHeight;

        // Get text dimensions (normalized units) for "Game Paused":
        AEGfxGetPrintSize(mFont, "Game Paused", 1.0f, &textWidth, &textHeight);

        // Center the text horizontally by subtracting half its width,
        // and center vertically by subtracting half its height from 0.0f
        // so that it’s in the exact middle of the screen.
        float centerX = -textWidth * 0.5f;
        float centerY = textHeight * 0.5f;

        // Print at scale = 1.0, with white color (1.0f,1.0f,1.0f,1.0f).
        AEGfxPrint(mFont, "Game Paused", centerX, centerY, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

        // Button center in world space:
        float resumeCenterX = player.x + kResumeX;
        float resumeCenterY = player.y + kResumeY;

        // Convert to normalized screen coordinates:
        float normX = (resumeCenterX - camX) / (winW / 2.0f);
        float normY = (resumeCenterY - camY) / (winH / 2.0f);

        // Get text dimensions (normalized units) for "Resume":
        AEGfxGetPrintSize(sFont, "Resume", 1.0f, &textWidth, &textHeight);

        // Offset so that the text's center aligns with the button center:
        normX -= textWidth / 2.0f;
        normY -= textHeight / 2.0f;

        // Draw the Resume button mesh:
        AEMtx33Trans(&t, player.x + kResumeX, player.y + kResumeY);
        AEGfxSetTransform(t.m);
        AEGfxMeshDraw(sResumeMesh, AE_GFX_MDM_TRIANGLES);

        // Print the centered text:
        AEGfxPrint(sFont, "Resume", normX, normY, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

        // --- Draw and center text on the Main Menu button ---
        // Button center in world space:
        float mainMenuCenterX = player.x + kMainMenuX;
        float mainMenuCenterY = player.y + kMainMenuY;

        // Convert to normalized coordinates:
        normX = (mainMenuCenterX - camX) / (winW / 2.0f);
        normY = (mainMenuCenterY - camY) / (winH / 2.0f);

        // Get text dimensions for "Main Menu":
        AEGfxGetPrintSize(sFont, "Main Menu", 1.0f, &textWidth, &textHeight);

        // Offset to center the text:
        normX -= textWidth / 2.0f;
        normY -= textHeight / 2.0f;

        // Draw the Main Menu button mesh:
        AEMtx33Trans(&t, player.x + kMainMenuX, player.y + kMainMenuY);
        AEGfxSetTransform(t.m);
        AEGfxMeshDraw(sMainMenuMesh, AE_GFX_MDM_TRIANGLES);

        // Print the centered text:
        AEGfxPrint(sFont, "Main Menu", normX, normY, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
    }

    void Free() {
        if (pauseScreenMesh) { AEGfxMeshFree(pauseScreenMesh); sResumeMesh = nullptr; }
        if (sResumeMesh) { AEGfxMeshFree(sResumeMesh); sResumeMesh = nullptr; }
        if (sMainMenuMesh) { AEGfxMeshFree(sMainMenuMesh); sMainMenuMesh = nullptr; }
        if (sFont != -1) { AEGfxDestroyFont(sFont); sFont = -1; }
    }

    bool IsPaused() { return sPaused; }

    void SetPaused(bool pause) { sPaused = pause; if (!pause) sExitToMenu = false; }

    bool ShouldExitToMenu() { return sExitToMenu; }
}
