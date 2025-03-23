/* Start Header ************************************************************************/
/*!
\file		gamePreview.cpp
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

#include "gamePreview.h"
#include "mainMenu.h"
#include "game1.h"
#include "AEEngine.h"
#include "Player.h"

gamePreview::gamePreview() : goGame(false), previewFont(-1), maze1(nullptr), maze1_mesh(nullptr), okButtonMesh(nullptr) {}

void gamePreview::Load() {
    AEGfxSetBackgroundColor(0.3f, 0.3f, 0.3f);

    previewFont = AEGfxCreateFont("Assets/liberation-mono.ttf", 70);

    maze1 = AEGfxTextureLoad("Assets/Maze1.png");

    if (maze1 == NULL) {
        printf("ERROR: Failed to load texture 'Assets/Maze1.png'\n");
    }

    maze1_mesh = TextureMesh();

    okButtonMesh = CreateButtonMesh(0xFFFFFFFF);
}

void gamePreview::Initialize() {
    AEInputReset();    // Reset any prior input events
    goGame = false;
}

void gamePreview::Update(f32 deltaTime) {
    (f32)deltaTime;

    // Example: Press ENTER to return to main menu
    if (AEInputCheckTriggered(AEVK_RETURN)) {
        goGame = true;
    }

    if (AEInputCheckTriggered(AEVK_LBUTTON)) {
        if (IsMouseInsideButton(0.0f, -300.0f, 200.0f, 100.0f)) {
            goGame = true;
        }
    }
}

void gamePreview::Draw() {
    AEGfxPrint(previewFont, "Maze Layout", -0.25f, 0.7f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);

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
    AEGfxTextureSet(maze1, 0, 0);

    // Create a 3x3 transformation matrix
    AEMtx33 transform;
    AEMtx33Identity(&transform);

    // Scale up the sprite (e.g., 2x in both dimensions)
    AEMtx33Scale(&transform, 500.0f, 500.0f);

    // Optionally, translate to reposition
    AEMtx33TransApply(&transform, &transform, 40.0f, 0.f);

    // Apply the transformation
    AEGfxSetTransform(transform.m);

    // Tell Alpha Engine to draw the mesh with the above settings.
    AEGfxMeshDraw(maze1_mesh, AE_GFX_MDM_TRIANGLES);

    //RenderMesh(maze1_mesh, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f); Works to render sprite

    if (!okButtonMesh) return;

    // Switch to color render mode
    AEGfxSetRenderMode(AE_GFX_RM_COLOR);

    RenderMesh(okButtonMesh, 0.0f, -300.0f, 1.0f, 1.0f, 0.0f);
    AEGfxPrint(previewFont, "OK", -0.055f, -0.72f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
}

void gamePreview::Free() {
    if (okButtonMesh) {
        AEGfxMeshFree(okButtonMesh);
        okButtonMesh = nullptr;
    }
    if (maze1_mesh) {
        AEGfxMeshFree(maze1_mesh);
        maze1_mesh = nullptr;
    }

}

void gamePreview::Unload() {
	if (previewFont != -1) {
		AEGfxDestroyFont(previewFont);
		previewFont = -1;
	}
	if (maze1) {
		AEGfxTextureUnload(maze1);
		maze1 = nullptr;
	}
}

BaseState* gamePreview::GetNextState() {
    // If goMenu is true, transition to main menu
    return goGame ? new GameState1() : nullptr;
}