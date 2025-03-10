/* Start Header ************************************************************************/
/*!
\file		mainMenu.cpp
\author		Henrey Hu
\email		zhihenghenrey.hu@digipen.edu

\brief      This file defines the MainMenuState class, which manages the game's main menu. 
            It handles user interactions, such as starting the game, accessing the guide, or exiting. 
            The file also includes utility functions for detecting mouse interactions with buttons and creating button meshes.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "AEEngine.h"
#include "mainMenu.h"
#include "gamePreview.h"
#include "instructions.h"
#include "Audio.h"
#include "Player.h"


bool IsMouseInsideButton(float buttonX, float buttonY, float buttonWidth, float buttonHeight) {
    s32 mouseX, mouseY;
    AEInputGetCursorPosition(&mouseX, &mouseY);

    // Convert screen space to world space
    f32 worldMouseX = (float)mouseX - (AEGfxGetWindowWidth() / 2);
    f32 worldMouseY = (AEGfxGetWindowHeight() / 2) - (float)mouseY;

    // Check if mouse is inside the rectangle
    return (worldMouseX >= buttonX - buttonWidth / 2 &&
        worldMouseX <= buttonX + buttonWidth / 2 &&
        worldMouseY >= buttonY - buttonHeight / 2 &&
        worldMouseY <= buttonY + buttonHeight / 2);
}

AEGfxVertexList* CreateButtonMesh(float width, float height, u32 color) {
    AEGfxMeshStart();

    // Create a rectangle using two triangles
    AEGfxTriAdd(-width / 2, -height / 2, color, 0.0f, 1.0f,
        width / 2, -height / 2, color, 1.0f, 1.0f,
        -width / 2, height / 2, color, 0.0f, 0.0f);

    AEGfxTriAdd(width / 2, -height / 2, color, 1.0f, 1.0f,
        width / 2, height / 2, color, 1.0f, 0.0f,
        -width / 2, height / 2, color, 0.0f, 0.0f);

    return AEGfxMeshEnd();
}

MainMenuState::MainMenuState()
    : startGame(false), menu(false), pGameRunning(nullptr), menuFont(-1), buttonFont(-1),
    playButtonMesh(nullptr), guideButtonMesh(nullptr), exitButtonMesh(nullptr) {}

void MainMenuState::Load() {
    AEGfxSetBackgroundColor(0.3f, 0.3f, 0.3f);

    // Use relative path instead of absolute path
    menuFont = AEGfxCreateFont("Assets/liberation-mono.ttf", 70);
    buttonFont = AEGfxCreateFont("Assets/liberation-mono.ttf", 50);

    if (menuFont == -1) {
        printf("ERROR: Failed to load font\n");
    }
    if (buttonFont == -1) {
        printf("ERROR: Failed to load font\n");
    }

    playButtonMesh = CreateButtonMesh(200.0f, 100.0f, 0xFFFFFFFF);
    guideButtonMesh = CreateButtonMesh(200.0f, 100.0f, 0xFFFFFFFF);
    exitButtonMesh = CreateButtonMesh(200.0f, 100.0f, 0xFFFFFFFF);

    AEAudioGroup MainMenuGroup = AEAudioCreateGroup();  // For background music
    AudioManager::LoadAudio("MainMenu", "Assets/MainMenu.wav", true, MainMenuGroup);
}

void MainMenuState::Initialize() {
    AEInputReset();    // Reset any prior input events
    startGame = false;
    menu = false;
    AEGfxSetCamPosition(0.0f, 0.0f);\
    AudioManager::PlayAudio("MainMenu", 1.0f, 1.0f, -1);

}

void MainMenuState::Update(f32 deltaTime) {
    (f32)deltaTime;

    // If user clicks the left mouse button, check if within the "Play" or "Exit"
    if (AEInputCheckTriggered(AEVK_LBUTTON)) {
        // Play button
        if (IsMouseInsideButton(0.0f, 100.0f, 200.0f, 100.0f)) {
            startGame = true;
        }
        // Guide button
        if (IsMouseInsideButton(0.0f, -50.0f, 200.0f, 100.0f)) {
            menu = true;
        }
        // Exit button
        if (IsMouseInsideButton(0.0f, -200.0f, 200.0f, 100.0f)) {
            // Safely quit by setting *pGameRunning = 0
            if (pGameRunning) {
                *pGameRunning = 0;
            }
        }
    }
}

void MainMenuState::Draw() {
    if (menuFont != -1) {
        AEGfxPrint(menuFont, "Wave", -0.10f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
    }

    if (!playButtonMesh || !guideButtonMesh || !exitButtonMesh) return;

    AEMtx33 transform;

    // Play Button (Center)
    AEMtx33Trans(&transform, 0.0f, 100.0f);
    AEGfxSetTransform(transform.m);
    AEGfxMeshDraw(playButtonMesh, AE_GFX_MDM_TRIANGLES);
    AEGfxPrint(buttonFont, "Play", -0.075f, 0.18f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);

    // Guide Button (Below Play Button)
    AEMtx33Trans(&transform, 0.0f, -50.0f);  // Moved down
    AEGfxSetTransform(transform.m);
    AEGfxMeshDraw(guideButtonMesh, AE_GFX_MDM_TRIANGLES);
    AEGfxPrint(buttonFont, "Guide", -0.095f, -0.15f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);

    // Exit Button (Below Guide Button)
    AEMtx33Trans(&transform, 0.0f, -200.0f);  // Moved down further
    AEGfxSetTransform(transform.m);
    AEGfxMeshDraw(exitButtonMesh, AE_GFX_MDM_TRIANGLES);
    AEGfxPrint(buttonFont, "Exit", -0.075f, -0.475f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
}

void MainMenuState::Free() {
    if (playButtonMesh) {
        AEGfxMeshFree(playButtonMesh);
        playButtonMesh = nullptr;
    }
    if (guideButtonMesh) {
        AEGfxMeshFree(guideButtonMesh);
        guideButtonMesh = nullptr;
    }
    if (exitButtonMesh) {
        AEGfxMeshFree(exitButtonMesh);
        exitButtonMesh = nullptr;
    }
}

void MainMenuState::Unload() {
    if (menuFont != -1) {
        AEGfxDestroyFont(menuFont);
        menuFont = -1;
    }
}

BaseState* MainMenuState::GetNextState() {
    if (startGame) {
        return new gamePreview();
    }
    if (menu) {
        return new instructionState();
    }
    return nullptr;
}