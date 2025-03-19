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

AEGfxVertexList* CreateButtonMesh(u32 color) {
    AEGfxMeshStart();

    // Create a rectangle using two triangles
    AEGfxTriAdd(-0.5f, -0.5f, color, 0.0f, 1.0f,
                0.5f, -0.5f, color, 1.0f, 1.0f,
                -0.5f, 0.5f, color, 0.0f, 0.0f);

    AEGfxTriAdd(0.5f, -0.5f, color, 1.0f, 1.0f,
                0.5f, 0.5f, color, 1.0f, 0.0f,
                -0.5f, 0.5f, color, 0.0f, 0.0f);

    return AEGfxMeshEnd();
}

// Utility function to draw buttons with scaling
void DrawButton(AEGfxVertexList* buttonMesh, float x, float y, const char* text, s8 font, float scaleX, float scaleY) {
    if (!buttonMesh || font == -1) return;

    // Create scaling and translation matrices
    AEMtx33 scaleMtx, transMtx, finalMtx;
    AEMtx33Scale(&scaleMtx, scaleX, scaleY);  // Scale the mesh
    AEMtx33Trans(&transMtx, x, y);            // Position the mesh
    AEMtx33Concat(&finalMtx, &transMtx, &scaleMtx);
    AEGfxSetTransform(finalMtx.m);
    AEGfxMeshDraw(buttonMesh, AE_GFX_MDM_TRIANGLES);

    // Retrieve camera position and window dimensions for text conversion
    float camX, camY;
    AEGfxGetCamPosition(&camX, &camY);
    float winW = (float)AEGfxGetWindowWidth();
    float winH = (float)AEGfxGetWindowHeight();

    // Get the text dimensions (assumes scale of 1.0 for printing)
    float textWidth, textHeight;
    AEGfxGetPrintSize(font, text, 1.0f, &textWidth, &textHeight);

    // Convert the world button center (x,y) to normalized screen coordinates,
    // subtracting half the text width/height to center the text.
    float normX = (x - camX) / (winW / 2.0f) - (textWidth * 0.5f);
    float normY = (y - camY) / (winH / 2.0f) - (textHeight * 0.5f);

    // Draw the text in normalized coordinates with black color
    AEGfxPrint(font, text, normX, normY, 1.0f, 0, 0, 0, 1);
}

MainMenuState::MainMenuState()
    : startGame(false), menu(false), pGameRunning(nullptr), menuFont(-1), buttonFont(-1),
    playButtonMesh(nullptr), guideButtonMesh(nullptr), exitButtonMesh(nullptr) {}

void MainMenuState::Load() {
    AEGfxSetBackgroundColor(0.f, 0.f, 0.f);

    // Use relative path instead of absolute path
    menuFont = AEGfxCreateFont("Assets/Fonts/Nosifer-Regular.ttf", 100);
    buttonFont = AEGfxCreateFont("Assets/Fonts/Nunito/Nunito-SemiBoldItalic.ttf", 50);

    if (menuFont == -1) {
        printf("ERROR: Failed to load font\n");
    }
    if (buttonFont == -1) {
        printf("ERROR: Failed to load font\n");
    }

    playButtonMesh = CreateButtonMesh(0xFFFFFFFF);
    guideButtonMesh = CreateButtonMesh(0xFFFFFFFF);
    exitButtonMesh = CreateButtonMesh(0xFFFFFFFF);
    AEAudioGroup BGMGROUP = AEAudioCreateGroup();  // For background music
    AudioManager::LoadAudio("MainMenu", "Assets/Audio/MainMenu.wav", true, BGMGROUP);
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
    if (!AudioManager::IsAudioPlaying("MainMenu")) {
        AudioManager::PlayAudio("MainMenu", 1.0f, 1.0f, -1);
    }
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
        float textWidth, textHeight;
        AEGfxGetPrintSize(menuFont, "WAVES", 1.0f, &textWidth, &textHeight);

        // Center X: 0.0f is the center of the screen, subtract half of text width
        float normX = -(textWidth * 0.5f);
        float normY = 0.5f;  // Keep Y position as before

        AEGfxPrint(menuFont, "WAVES", normX, normY, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f);
    }

    if (!playButtonMesh || !guideButtonMesh || !exitButtonMesh) return;

    DrawButton(playButtonMesh, 0.0f, 100.0f, "Play", buttonFont, 200.0f, 100.0f);
    DrawButton(guideButtonMesh, 0.0f, -50.0f, "Guide", buttonFont, 200.0f, 100.0f);
    DrawButton(exitButtonMesh, 0.0f, -200.0f, "Exit", buttonFont, 200.0f, 100.0f);
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
    AudioManager::UnloadAudio("MainMenu");
}

BaseState* MainMenuState::GetNextState() {
    if (startGame) {
        return new GameState1();
        // return new gamePreview();
    }
    if (menu) {
        return new instructionState();
    }
    return nullptr;
}