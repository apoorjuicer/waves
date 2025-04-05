/* Start Header ************************************************************************/
/*!
\file		gamePreview.cpp
\author		Henrey Hu
\co-author  Chewn Thing Kwan
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

// Constructor
gamePreview::gamePreview(int i) : goGame(false), back2Main(false), previewFont(-1), maze1_mesh(nullptr), level_num{ i } {}

// Load assets for the game preview
void gamePreview::Load() {
    AEGfxSetCamPosition(0, 0);

    AEGfxSetBackgroundColor(0.3f, 0.3f, 0.3f);

    previewFont = FontManager::GetFont("NunitoBold70");

    maze1_mesh = TextureMesh();

    // read file depending on which level selected;
    switch (level_num) {
    case 1:
        if (!ImportMapDataFromFile("Assets/Maze1.txt")) {
            std::cerr << "Error: Failed to load maze file Assets/Maze1.txt. Quitting game.\n";
        }
        break;
    case 2:
        if (!ImportMapDataFromFile("Assets/Maze2.txt")) {
            std::cerr << "Error: Failed to load maze file Assets/Maze2.txt. Quitting game.\n";
        }
        break;
    case 3:
        if (!ImportMapDataFromFile("Assets/Maze3.txt")) {
            std::cerr << "Error: Failed to load maze file Assets/Maze3.txt. Quitting game.\n";
        }
        break;
    }
}

// Initialize the game preview state
void gamePreview::Initialize() {
    goGame = false;
    back2Main = false;
}

// Update the game preview state
void gamePreview::Update(f32 deltaTime) {
    (f32)deltaTime;

    if (AEInputCheckTriggered(AEVK_RETURN)) {
        goGame = true;
    }

	if (AEInputCheckTriggered(AEVK_ESCAPE)) {
		back2Main = true;
	}

    if (AEInputCheckTriggered(AEVK_LBUTTON)) {
        // ---------------------------
        // "OK" Button
        // ---------------------------
        {
            f32 textW, textH;
            AEGfxGetPrintSize(previewFont, "OK", 1.0f, &textW, &textH);

            f32 centerX_NDC = 0.0f;
            f32 centerY_NDC = -0.7f;

            if (IsMouseInsideButtonNDC(centerX_NDC, centerY_NDC, textW, textH))
            {
                goGame = true;
            }
        }
    }
}

// Draw the game preview state
void gamePreview::Draw() {
    // ======================================================================
    // RENDER MAP PREVIEW
    // ======================================================================
    // find scale of each pixel depending on height of maze;
    const f32 p_scale = (900.f - 400.f) / activeRow;
    // find position of first pixel;
    AEVec2 start_pos{};
    start_pos.x = -((p_scale * activeCol / 2.f) - (p_scale / 2.f));
    start_pos.y = (p_scale * activeRow / 2.f) - (p_scale / 2.f);

    // switch to color render mode;
    AEGfxSetRenderMode(AE_GFX_RM_COLOR);

    for (int y{}; y < activeRow; ++y) {
        for (int x{}; x < activeCol; ++x) {
            
            // set color depending on maze tile; if tile is not wall or goal, skip tile;
            switch (maze[y][x]) {
            case 1:
                AEGfxSetColorToMultiply(0.0f, 0.0f, 0.0f, 1.0f);
                break;
            case 2:
                AEGfxSetColorToMultiply(0.0f, 1.0f, 0.0f, 1.0f);
                break;
            default:
                AEGfxSetColorToMultiply(0.2f, 0.2f, 0.2f, 0.2f);
            }

            // apply transform for current tile;
            // create a 3x3 transformation matrix;
            AEMtx33 transform{}, scale{}, trans{};
            AEMtx33Scale(&scale, p_scale, p_scale);
            AEMtx33Trans(&trans, start_pos.x, start_pos.y);
            // set transform;
            AEMtx33Concat(&transform, &trans, &scale);
            AEGfxSetTransform(transform.m);
            // draw;
            AEGfxMeshDraw(maze1_mesh, AE_GFX_MDM_TRIANGLES);

            // update position of next pixel;
            start_pos.x += p_scale;
            AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
        }
        // update position of next pixel after ending row;
        start_pos.x = -((p_scale * activeCol / 2.f) - (p_scale / 2.f));
        start_pos.y -= p_scale;
    }
    
    // ======================================================================
	// Print Maze Layout Text
    // ======================================================================
    f32 TextWidthNDC, TextHeightNDC;
    AEGfxGetPrintSize(previewFont, "Maze Layout", 1.0f, &TextWidthNDC, &TextHeightNDC);
    f32 CenterX_NDC = 0.0f;
    f32 CenterY_NDC = 0.75f;
    f32 TextX_NDC = CenterX_NDC - (TextWidthNDC * 0.5f);
    f32 TextY_NDC = CenterY_NDC - (TextHeightNDC * 0.5f);
    AEGfxPrint(previewFont, "Maze Layout", TextX_NDC, TextY_NDC, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);

	// ======================================================================
	// RENDER OK BUTTON
	// ======================================================================
    {
        f32 OKTextWidthNDC, OKTextHeightNDC;
        AEGfxGetPrintSize(previewFont, "OK", 1.0f, &OKTextWidthNDC, &OKTextHeightNDC);

        f32 OKCenterX_NDC = 0.0f;
        f32 OKCenterY_NDC = -0.7f;

        f32 OK_r = 0.0f;
        f32 OK_g = 0.0f;
        f32 OK_b = 0.0f;

        f32 OKTextX_NDC = OKCenterX_NDC - (OKTextWidthNDC * 0.5f);
        f32 OKTextY_NDC = OKCenterY_NDC - (OKTextHeightNDC * 0.5f);
        AEGfxPrint(previewFont, "OK", OKTextX_NDC, OKTextY_NDC, 1.0f, OK_r, OK_g, OK_b, 1.0f);
    }
}

// Free runtime resources
void gamePreview::Free() {
    if (maze1_mesh) {
        AEGfxMeshFree(maze1_mesh);
        maze1_mesh = nullptr;
    }
}

// Unload assets for the game preview
void gamePreview::Unload() {}

// Return the next state
BaseState* gamePreview::GetNextState() {
	// If back2Main is true, transition to main menu
	if (back2Main) {
		return new LevelSelectState();
    }

    // If goMenu is true, transition to main menu
    switch (level_num) {
    case 1:
        return goGame ? new GameState1() : nullptr;
    case 2:
        return goGame ? new GameState2() : nullptr;
    case 3:
        return goGame ? new GameState3() : nullptr;
	default:
        return nullptr;
    }
}