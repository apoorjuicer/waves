/* Start Header ************************************************************************/
/*!
\file		exitOverlay.cpp
\project    Waves

\author		Henrey Hu
\email		zhihenghenrey.hu@digipen.edu

\brief      This file defines the ExitConfirmOverlay namespace, which manages the exit confirmation overlay in the game.
            It provides functionality for initializing, updating, drawing, and freeing the overlay resources.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "exitOverlay.h"

// ======================================================
// Internal variables for the exit confirmation overlay
// ======================================================
namespace {
	bool sVisible = false;
	AEGfxVertexList* panelMesh = nullptr; 
	AEGfxVertexList* yesButtonMesh = nullptr;
	AEGfxVertexList* noButtonMesh = nullptr; 
    s8 font = -1;

	const f32 panelWidth = 500.0f;
    const f32 panelHeight = 300.0f;
    const f32 panelPosX = 0.0f;
    const f32 panelPosY = 0.0f;

    const f32 buttonW = 150.0f;
    const f32 buttonH = 60.0f;

    const f32 yesX = -100.0f;
    const f32 noX = 100.0f;
    const f32 buttonY = -80.0f;
}

// ======================================================
// Internal functions for the exit confirmation overlay
// ======================================================
namespace ExitConfirmOverlay {
	// Initialize the overlay
    void Init() {
        sVisible = false;
        font = FontManager::GetFont("NunitoBold50");
        panelMesh = CreateButtonMesh(0xFF202020);
        yesButtonMesh = CreateButtonMesh(0xFFFFFFFF);
        noButtonMesh = CreateButtonMesh(0xFFFFFFFF);
    }

	// Update the overlay
    void Update(int* pGameRunning) {
        if (!sVisible) return;

		// Check if the mouse is inside the button
        if (AEInputCheckTriggered(AEVK_LBUTTON)) {
            if (IsMouseInsideButton(yesX, buttonY, buttonW, buttonH)) {
                if (pGameRunning)
                    *pGameRunning = 0;
            }
            if (IsMouseInsideButton(noX, buttonY, buttonW, buttonH)) {
                sVisible = false;
            }
        }
    }

	// Draw the overlay
    void Draw() {
        if (!sVisible) return;

        // Panel
        AEMtx33 t, s, m;
        AEMtx33Trans(&t, panelPosX, panelPosY);
        AEMtx33Scale(&s, panelWidth, panelHeight);
        AEMtx33Concat(&m, &t, &s);
        AEGfxSetTransform(m.m);
        AEGfxMeshDraw(panelMesh, AE_GFX_MDM_TRIANGLES);

        // Text
        float textW, textH;
        AEGfxGetPrintSize(font, "Exit Game?", 1.0f, &textW, &textH);
        AEGfxPrint(font, "Exit Game?", -textW / 2, 0.2f, 1.0f, 1, 1, 1, 1);

        // Yes button
        DrawButton(yesButtonMesh, yesX, buttonY, "Yes", font, buttonW, buttonH, 0, 0, 0, 1);
        // No button
        DrawButton(noButtonMesh, noX, buttonY, "No", font, buttonW, buttonH, 0, 0, 0, 1);
    }

	// Free the overlay resources
    void Free() {
        if (panelMesh) { AEGfxMeshFree(panelMesh); panelMesh = nullptr; }
        if (yesButtonMesh) { AEGfxMeshFree(yesButtonMesh); yesButtonMesh = nullptr; }
        if (noButtonMesh) { AEGfxMeshFree(noButtonMesh); noButtonMesh = nullptr; }
    }

    // Toggle the overlay visibility
    void Toggle() {
        sVisible = !sVisible;
    }

    // Check if the overlay is visible
    bool IsVisible() {
        return sVisible;
    }
}
