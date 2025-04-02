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
    const f32 kResumeY = -120.0f;
    const f32 kResumeX = 0.0f;
    const f32 kMainMenuY = -250.0f;
    const f32 kMainMenuX = 0.0f;
    const f32 pauseMenuX = 1000.0f;
    const f32 pauseMenuY = 700.0f;
}

namespace PauseOverlay {
    void Init() {
        sPaused = false;
        sExitToMenu = false;

        /*sFont = AEGfxCreateFont("Assets/Fonts/Nunito/Nunito-Bold.ttf", 50);
        mFont = AEGfxCreateFont("Assets/Fonts/Nunito/Nunito-Bold.ttf", 100);*/

        sFont = FontManager::GetFont("NunitoBold50");
        mFont = FontManager::GetFont("NunitoBold100");

        pauseScreenMesh = CreateButtonMesh(0xFF404040);
        sResumeMesh = CreateButtonMesh(0xFF606060);
        sMainMenuMesh = CreateButtonMesh(0xFF606060);
    }

    void Update() {
        if (!sPaused) return;
        if (AEInputCheckTriggered(AEVK_LBUTTON)) {
            if (IsMouseInsideButton(kResumeX, kResumeY, kButtonW, kButtonH)) {
                sPaused = false;
            }
            else if (IsMouseInsideButton(kMainMenuX, kMainMenuY, kButtonW, kButtonH)) {
                sExitToMenu = true;
                sPaused = false;
            }
        }
    }

    void Draw() {
        if (!sPaused) return;

        // Reset any lingering texture from previous draws
        AEGfxTextureSet(nullptr, 0, 0);

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
        AEGfxGetPrintSize(mFont, "GAME PAUSED", 1.0f, &textWidth, &textHeight);

        // Center the text horizontally and vertically (normalized coordinates)
        float centerX = -textWidth * 0.5f;
        float centerY = textHeight * 0.5f;
        AEGfxPrint(mFont, "GAME PAUSED", centerX, centerY, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

        // --- Draw the Resume Button using DrawButton() ---
        DrawButton(sResumeMesh, player.x + kResumeX, player.y + kResumeY, "Resume", sFont, 300.0f, 100.0f, 1.0f, 1.0f, 1.0f, 1.0f);

        // --- Draw the Main Menu Button using DrawButton() ---
        DrawButton(sMainMenuMesh, player.x + kMainMenuX, player.y + kMainMenuY, "Main Menu", sFont, 300.0f, 100.0f, 1.0f, 1.0f, 1.0f, 1.0f);
    }

    void Free() {
        if (pauseScreenMesh) { AEGfxMeshFree(pauseScreenMesh); sResumeMesh = nullptr; }
        if (sResumeMesh) { AEGfxMeshFree(sResumeMesh); sResumeMesh = nullptr; }
        if (sMainMenuMesh) { AEGfxMeshFree(sMainMenuMesh); sMainMenuMesh = nullptr; }
        //if (sFont != -1) { AEGfxDestroyFont(sFont); sFont = -1; }
    }

    bool IsPaused() { return sPaused; }

    void SetPaused(bool pause) { sPaused = pause; if (!pause) sExitToMenu = false; }

    bool ShouldExitToMenu() { return sExitToMenu; }
}
