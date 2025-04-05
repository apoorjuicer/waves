/* Start Header ************************************************************************/
/*!
\file		credits.cpp
\author		Henrey Hu
\email		zhihenghenrey.hu@digipen.edu

\brief      This file defines the creditState class, which manages the display of game credits.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "credits.h"

// Constructor
creditState::creditState() : goMenu(false), credit(nullptr), credit_mesh(nullptr) {}

// Load assets for the credit state
void creditState::Load() {
    AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

    credit = AEGfxTextureLoad("Assets/Credits.png");

    credit_mesh = TextureMesh();
}

// Initialize the credit state
void creditState::Initialize() {
    goMenu = false;
}

// Update the credit state
void creditState::Update(f32 deltaTime) {
    (f32)deltaTime;

    if (AEInputCheckTriggered(AEVK_ESCAPE)) {
        goMenu = true;
    }
}

void creditState::Draw() {
	// ============================
	// Draw the credits
	// ============================
    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
    AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
    AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
    AEGfxSetBlendMode(AE_GFX_BM_BLEND);
    AEGfxSetTransparency(1.0f);
    AEGfxTextureSet(credit, 0, 0);

    AEMtx33 transform;
    AEMtx33Identity(&transform);

    int winWidth = AEGfxGetWindowWidth();
    int winHeight = AEGfxGetWindowHeight();

    AEMtx33Scale(&transform, (f32)winWidth, (f32)winHeight);

    // Optionally, translate to reposition
    AEMtx33TransApply(&transform, &transform, 0.0f, 0.0f);

    // Apply the transformation
    AEGfxSetTransform(transform.m);

    // Tell Alpha Engine to draw the mesh with the above settings.
    AEGfxMeshDraw(credit_mesh, AE_GFX_MDM_TRIANGLES);
}

// Free runtime resources
void creditState::Free()
{
    if (credit_mesh)
    {
        AEGfxMeshFree(credit_mesh);
        credit_mesh = nullptr;
    }
}

// Unload assets for the credit state
void creditState::Unload() {
    if (credit)
    {
        AEGfxTextureUnload(credit);
        credit = nullptr;
    }
}

// Return the next state
BaseState* creditState::GetNextState() {
    // If goMenu is true, transition to main menu
    return goMenu ? new MainMenuState() : nullptr;
}