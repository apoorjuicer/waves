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

gamePreview::gamePreview(int i) : goGame(false), previewFont(-1), maze1_mesh(nullptr), okButtonMesh(nullptr), level_num{ i } {}

void gamePreview::Load() {
    AEGfxSetCamPosition(0, 0);

    AEGfxSetBackgroundColor(0.3f, 0.3f, 0.3f);

    //previewFont = AEGfxCreateFont("Assets/Fonts/Nunito/Nunito-Bold.ttf", 70);
    previewFont = FontManager::GetFont("NunitoBold70");

    maze1_mesh = TextureMesh();

    okButtonMesh = CreateButtonMesh(0xFFFFFFFF);

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
    
    // print text;
    AEGfxPrint(previewFont, "Maze Layout", -0.25f, 0.7f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);

    if (!okButtonMesh) return;
    
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
	/*if (previewFont != -1) {
		AEGfxDestroyFont(previewFont);
		previewFont = -1;
	}*/
}

BaseState* gamePreview::GetNextState() {
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