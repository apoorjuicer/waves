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
        pauseScreenMesh = CreateButtonMesh(0xFF404040);
        sResumeMesh = CreateButtonMesh(0xFF202020);
        sMainMenuMesh = CreateButtonMesh(0xFF202020);
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

        AEMtx33 t, scaleMtx, finalMtx;

        // Draw pause screen background (using player's world position as base)
        AEMtx33Trans(&t, player.x, player.y);
        AEMtx33Scale(&scaleMtx, pauseMenuX, pauseMenuY);
        AEMtx33Concat(&finalMtx, &t, &scaleMtx);
        AEGfxSetTransform(finalMtx.m);
        AEGfxMeshDraw(pauseScreenMesh, AE_GFX_MDM_TRIANGLES);

        // --- Draw and center text on the Game Paused Text ---
        //float winW = (float)AEGfxGetWindowWidth();
        //float winH = (float)AEGfxGetWindowHeight();
        float camX, camY;
        AEGfxGetCamPosition(&camX, &camY);
        float textWidth, textHeight;

        // Get text dimensions (normalized units) for "Game Paused":
        AEGfxGetPrintSize(mFont, "Game Paused", 1.0f, &textWidth, &textHeight);

        // Center the text horizontally and vertically (normalized coordinates)
        float centerX = -textWidth * 0.5f;
        float centerY = textHeight * 0.5f;
        AEGfxPrint(mFont, "Game Paused", centerX, centerY, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

        // --- Draw the Resume Button using DrawButton() ---
        DrawButton(sResumeMesh, player.x + kResumeX, player.y + kResumeY, "Resume", sFont, 300.0f, 100.0f);

        // --- Draw the Main Menu Button using DrawButton() ---
        DrawButton(sMainMenuMesh, player.x + kMainMenuX, player.y + kMainMenuY, "Main Menu", sFont, 300.0f, 100.0f);
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
