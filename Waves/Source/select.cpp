/* Start Header ************************************************************************/
/*!
\file		select.cpp
\author		Henrey Hu
\email		zhihenghenrey.hu@digipen.edu

\brief      This file defines the selectState class, which manages the level selection screen.
			It handles loading, rendering, and user interactions, allowing players to choose a level to play.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "select.h"

const f32 ButtonW = 100.0f;
const f32 ButtonH = 100.0f;
const f32 gamebuttonY = 0.0f;
const f32 game1X = -300.0f;
const f32 game2X = 0.0f;
const f32 game3X = 300.0f;
static const float BACK_BTN_CENTER_X_NDC = -0.95f; 
static const float BACK_BTN_CENTER_Y_NDC = 0.95f;
f32 buttonSize = 0.0f;

// Constructor
LevelSelectState::LevelSelectState() :  game1(nullptr), game2(nullptr), game3(nullptr), back2main(nullptr), 
                                        selectFont(-1), 
                                        game1ButtonMesh(nullptr), game2ButtonMesh(nullptr), game3ButtonMesh(nullptr), back2mainButtonMesh(nullptr) {}

// Load assets for the level select state
void LevelSelectState::Load() {
    AEGfxSetBackgroundColor(0.f, 0.f, 0.f);

    selectFont = FontManager::GetFont("NunitoMediumItalic100");

    game1ButtonMesh =       CreateButtonMesh(0xFFFFFFFF);
    game2ButtonMesh =       CreateButtonMesh(0xFFFFFFFF);
    game3ButtonMesh =       CreateButtonMesh(0xFFFFFFFF);
    back2mainButtonMesh =   CreateButtonMesh(0xFFFFFFFF);

    pHomeIconTexture = AEGfxTextureLoad("Assets/Home Icon.png");
}

// Initialize the level select state
void LevelSelectState::Initialize() {
    // Reset state data.
    game1 = false;
    game2 = false;
    game3 = false;
    back2main = false;
}

// Update the level select state
void LevelSelectState::Update(f32 deltaTime) 
{
    (f32)deltaTime;

    if (AEInputCheckTriggered(AEVK_ESCAPE)) {
        back2main = true;
    }

    f32 halfW = (float)AEGfxGetWindowWidth() * 0.5f;
    f32 halfH = (float)AEGfxGetWindowHeight() * 0.5f;

    float leftNDC = -1.0f;
    float topNDC = 1.0f;

    float leftWorld = leftNDC * halfW;
    float topWorld = topNDC * halfH;

    float centerX = leftWorld + (buttonSize * 0.5f);
    float centerY = topWorld - (buttonSize * 0.5f);

    float windowW = (float)AEGfxGetWindowWidth();
    float windowH = (float)AEGfxGetWindowHeight();
    float smallerDim = (windowW < windowH) ? windowW : windowH;

    buttonSize = smallerDim * 0.05f;

    if (AEInputCheckTriggered(AEVK_LBUTTON)) {
        if (IsMouseInsideButton(game1X, gamebuttonY, ButtonW, ButtonH)) {
            game1 = true;
        }
        else if (IsMouseInsideButton(game2X, gamebuttonY, ButtonW, ButtonH)) {
            game2 = true;
        }
        else if (IsMouseInsideButton(game3X, gamebuttonY, ButtonW, ButtonH)) {
            game3 = true;
        }
        else if (IsMouseInsideButton(centerX, centerY, buttonSize, buttonSize)) {
            back2main = true;
        }
    }
}

// Draw the level select state
void LevelSelectState::Draw() 
{
    float textWidth, textHeight;
    AEGfxGetPrintSize(selectFont, "Levels", 1.0f, &textWidth, &textHeight);

    float centerX = -textWidth * 0.5f;
    float centerY = textHeight * 0.5f + 0.5f;

    AEGfxPrint(selectFont, "Levels", centerX, centerY, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    // Draw button "1"
    DrawButton(game1ButtonMesh, game1X, gamebuttonY, "1", selectFont, ButtonW, ButtonH, 0.0f, 0.0f, 0.0f, 1.0f);

    // Draw button "2" 
    DrawButton(game2ButtonMesh, game2X, gamebuttonY, "2", selectFont, ButtonW, ButtonH, 0.0f, 0.0f, 0.0f, 1.0f);

    // Draw button "3"
    DrawButton(game3ButtonMesh, game3X, gamebuttonY, "3", selectFont, ButtonW, ButtonH, 0.0f, 0.0f, 0.0f, 1.0f);

    f32 halfW = (float)AEGfxGetWindowWidth() * 0.5f;
    f32 halfH = (float)AEGfxGetWindowHeight() * 0.5f;

    // If you want the top-left corner at (-0.95, +0.95) in normalized space:
    float leftNDC = -1.0f;
    float topNDC = 1.0f;

    // Convert to world/pixel
    float leftWorld = leftNDC * halfW;
    float topWorld = topNDC * halfH;

    // Draw Exit to Menu
    DrawTexturedButton(back2mainButtonMesh,
        leftWorld, topWorld,
        pHomeIconTexture,
        buttonSize, buttonSize);
}

// Free runtime resources
void LevelSelectState::Free() {
    if (game1ButtonMesh) {
        AEGfxMeshFree(game1ButtonMesh);
        game1ButtonMesh = nullptr;
    }
    if (game2ButtonMesh) {
        AEGfxMeshFree(game2ButtonMesh);
        game2ButtonMesh = nullptr;
    }
    if (game3ButtonMesh) {
        AEGfxMeshFree(game3ButtonMesh);
        game3ButtonMesh = nullptr;
    }
    if (back2mainButtonMesh) {
        AEGfxMeshFree(back2mainButtonMesh);
        back2mainButtonMesh = nullptr;
    }
}

// Unload assets for the level select state
void LevelSelectState::Unload() {
    AEGfxTextureUnload(pHomeIconTexture);
}

// Return the next state
BaseState* LevelSelectState::GetNextState() 
{
    if (game1) {
        //return new GameState1();
        return new gamePreview(1);
    }
    if (game2) {
        //return new GameState2();
        return new gamePreview(2);
    }
    if (game3) {
        //return new GameState3();
        return new gamePreview(3);
    }
    if (back2main) {
        return new MainMenuState();
    }
    return nullptr;
}
