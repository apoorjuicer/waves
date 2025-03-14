/* Start Header ************************************************************************/
/*!
\file		instructions.cpp
\author		Henrey Hu
\email		zhihenghenrey.hu@digipen.edu

\brief      This file defines the instructionState class, which manages the display of game instructions. 
            It handles loading, rendering, and user interactions, 
            allowing players to view gameplay guidelines before returning to the main menu.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "instructions.h"
#include "mainMenu.h"
#include "AEEngine.h"

instructionState::instructionState() : goMenu(false), guide(nullptr), guide_mesh(nullptr) {}

void instructionState::Load() {
    AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

    guide = AEGfxTextureLoad("Assets/Guide.png");

    guide_mesh = TextureMesh();
}

void instructionState::Initialize() {
    goMenu = false;
}

void instructionState::Update(f32 deltaTime) {
    (f32)deltaTime;

    // Example: Press ENTER to return to main menu
    if (AEInputCheckTriggered(AEVK_ESCAPE)) {
        goMenu = true;
    }
}

void instructionState::Draw() {
    // Tell the Alpha Engine to get ready to draw something with texture.
    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

    // Set the the color to multiply to white, so that the sprite can 
    // display the full range of colors (default is black).
    AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);

    // Set the color to add to nothing, so that we don't alter the sprite's color
    AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);

    // Set blend mode to AE_GFX_BM_BLEND, which will allow transparency.
    AEGfxSetBlendMode(AE_GFX_BM_BLEND);
    AEGfxSetTransparency(1.0f);

    // Tell Alpha Engine to use the texture stored in pTex
    AEGfxTextureSet(guide, 0, 0);

    // Create a 3x3 transformation matrix
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
    AEGfxMeshDraw(guide_mesh, AE_GFX_MDM_TRIANGLES);
}

void instructionState::Free() {}

void instructionState::Unload() {}

BaseState* instructionState::GetNextState() {
    // If goMenu is true, transition to main menu
    return goMenu ? new MainMenuState() : nullptr;
}