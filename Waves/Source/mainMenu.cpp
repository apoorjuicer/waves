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

#include "mainMenu.h"

MainMenuState::MainMenuState()
    : startGame(false), menu(false), pGameRunning(nullptr), menuFont(-1), buttonFont(-1),
    playButtonMesh(nullptr), guideButtonMesh(nullptr), exitButtonMesh(nullptr), creditButtonMesh(nullptr), settingButtonMesh(nullptr) {}

void MainMenuState::Load() {
    AEGfxSetBackgroundColor(0.f, 0.f, 0.f);

    // Use relative path instead of absolute path
    //menuFont = AEGfxCreateFont("Assets/Fonts/Nosifer-Regular.ttf", 150);
    //buttonFont = AEGfxCreateFont("Assets/Fonts/Nunito/Nunito-SemiBoldItalic.ttf", 70);

    menuFont = FontManager::GetFont("NosiferRegular150");
    buttonFont = FontManager::GetFont("NunitoSemiBoldItalic70");

    playButtonMesh = CreateButtonMesh(0xFFFFFFFF);
    guideButtonMesh = CreateButtonMesh(0xFFFFFFFF);
    creditButtonMesh = CreateButtonMesh(0xFFFFFFFF);
    settingButtonMesh = CreateButtonMesh(0xFFFFFFFF);
    exitButtonMesh = CreateButtonMesh(0xFFFFFFFF);

    AEAudioGroup BGMGROUP = AEAudioCreateGroup();  // For background music
    AudioManager::LoadAudio("MainMenu", "Assets/Audio/MainMenu.wav", true, BGMGROUP);
}

void MainMenuState::Initialize() {
    AEInputReset();    // Reset any prior input events
    startGame = false;
    menu = false;
    waitingForMouseRelease = true;
    AEGfxSetCamPosition(0.0f, 0.0f);
    AudioManager::PlayAudio("MainMenu", 1.0f, 1.0f, -1);
}

void MainMenuState::Update(f32 deltaTime) {
    (f32)deltaTime;

    if (!AudioManager::IsAudioPlaying("MainMenu")) {
        AudioManager::PlayAudio("MainMenu", 1.0f, 1.0f, -1);
    }
    
	// Prevent multiple clicks
    if (waitingForMouseRelease) {
        if (!AEInputCheckCurr(AEVK_LBUTTON)) {
            waitingForMouseRelease = false;
        }
        return;
    }

    // Only check clicks when the left mouse button is triggered.
    if (AEInputCheckTriggered(AEVK_LBUTTON))
    {
        // ---------------------------
        // "Play" Button (NDC positions)
        // ---------------------------
        {
            f32 textW, textH;
            AEGfxGetPrintSize(buttonFont, "Play", 1.0f, &textW, &textH);

            // Compute text center 
            f32 centerX_NDC = 0.0f;
            f32 centerY_NDC = 0.3f;

            // If click is within the text's bounding box, trigger the Play action.
            if (IsMouseInsideButtonNDC(centerX_NDC, centerY_NDC, textW, textH))
            {
                startGame = true;
            }
        }

        // ---------------------------
        // "Guide" Button
        // ---------------------------
        {
            f32 textW, textH;
            AEGfxGetPrintSize(buttonFont, "Guide", 1.0f, &textW, &textH);

            f32 centerX_NDC = 0.0f;
            f32 centerY_NDC = 0.0f;

            if (IsMouseInsideButtonNDC(centerX_NDC, centerY_NDC, textW, textH))
            {
                menu = true;
            }
        }

        // ---------------------------
        // "Credit" Button
        // ---------------------------
        {
            f32 textW, textH;
            AEGfxGetPrintSize(buttonFont, "Credit", 1.0f, &textW, &textH);

            f32 centerX_NDC = 0.0f;
            f32 centerY_NDC = -0.3f;

            if (IsMouseInsideButtonNDC(centerX_NDC, centerY_NDC, textW, textH))
            {
                // Trigger the Credit state here, for example:
                // currentState = CREDIT_STATE;
            }
        }

        // ---------------------------
        // "Setting" Button
        // ---------------------------
        {
            f32 textW, textH;
            AEGfxGetPrintSize(buttonFont, "Setting", 1.0f, &textW, &textH);

            f32 centerX_NDC = 0.0f;
            f32 centerY_NDC = -0.6f;

            if (IsMouseInsideButtonNDC(centerX_NDC, centerY_NDC, textW, textH))
            {
                // Trigger the Settings state here, for example:
                // currentState = SETTINGS_STATE;
            }
        }

        // ---------------------------
        // "Exit Game" Button
        // ---------------------------
        {
            f32 textW, textH;
            AEGfxGetPrintSize(buttonFont, "Exit Game", 1.0f, &textW, &textH);

            f32 centerX_NDC = 0.0f;
            f32 centerY_NDC = -0.9f;

            if (IsMouseInsideButtonNDC(centerX_NDC, centerY_NDC, textW, textH))
            {
                // Safely exit the game.
                if (pGameRunning)
                    *pGameRunning = 0;
            }
        }
    }
}

void MainMenuState::Draw() {
    if (menuFont != -1) {
        f32 textWidth, textHeight;
        AEGfxGetPrintSize(menuFont, "WAVES", 1.0f, &textWidth, &textHeight);

        // Center X: 0.0f is the center of the screen, subtract half of text width
        f32 normX = -(textWidth * 0.5f);
        f32 normY = 0.6f;  // Keep Y position as before

        AEGfxPrint(menuFont, "WAVES", normX, normY, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f);
    }

    if (!playButtonMesh || !guideButtonMesh || !exitButtonMesh) return;

    // ==============================================================
    // Color Change on Hover 'Play' Button
    // ==============================================================    
    {
        f32 PlaytextWidthNDC, PlaytextHeightNDC;
        AEGfxGetPrintSize(buttonFont, "Play", 1.0f, &PlaytextWidthNDC, &PlaytextHeightNDC);

        // Set the text's center to be horizontally centered (0.0f) and at desired vertical position:
        f32 centerX_NDC = 0.0f;          // Center horizontally in NDC
        f32 centerY_NDC = 0.3f;         // Your desired vertical position in NDC

        // Optionally, check hover for color change:
        bool Playhovered = IsMouseInsideButtonNDC(centerX_NDC, centerY_NDC, PlaytextWidthNDC, PlaytextHeightNDC);
        f32 Play_r = (Playhovered) ? 0.5f : 1.0f;
        f32 Play_g = (Playhovered) ? 0.0f : 1.0f;
        f32 Play_b = (Playhovered) ? 0.0f : 1.0f;

        // Calculate the top-left position for printing (offset the center by half text dimensions)
        f32 PlayTextX_NDC = centerX_NDC - (PlaytextWidthNDC * 0.5f);
        f32 PlayTextY_NDC = centerY_NDC - (PlaytextHeightNDC * 0.5f);
        AEGfxPrint(buttonFont, "Play", PlayTextX_NDC, PlayTextY_NDC, 1.0f, Play_r, Play_g, Play_b, 1.0f);
    }

    // ==============================================================
    // Color Change on Hover 'Guide' Button
    // ==============================================================    
    {
        f32 GuidetextWidthNDC, GuidetextHeightNDC;
        AEGfxGetPrintSize(buttonFont, "Guide", 1.0f, &GuidetextWidthNDC, &GuidetextHeightNDC);

        f32 GuidecenterX_NDC = 0.0f;
        f32 GuidecenterY_NDC = -0.0f;

        bool Guidehovered = IsMouseInsideButtonNDC(GuidecenterX_NDC, GuidecenterY_NDC, GuidetextWidthNDC, GuidetextHeightNDC);
        f32 Guide_r = (Guidehovered) ? 0.5f : 1.0f;
        f32 Guide_g = (Guidehovered) ? 0.0f : 1.0f;
        f32 Guide_b = (Guidehovered) ? 0.0f : 1.0f;

        f32 GuidetextX_NDC = GuidecenterX_NDC - (GuidetextWidthNDC * 0.5f);
        f32 GuidetextY_NDC = GuidecenterY_NDC - (GuidetextHeightNDC * 0.5f);
        AEGfxPrint(buttonFont, "Guide", GuidetextX_NDC, GuidetextY_NDC, 1.0f, Guide_r, Guide_g, Guide_b, 1.0f);
    }

    // ==============================================================
    // Color Change on Hover 'Credit' Button
    // ==============================================================    
    {
        f32 CreditTextWidthNDC, CreditTextHeightNDC;
        AEGfxGetPrintSize(buttonFont, "Credit", 1.0f, &CreditTextWidthNDC, &CreditTextHeightNDC);

        f32 CreditCenterX_NDC = 0.0f;
        f32 CreditCenterY_NDC = -0.3f;

        bool CreditHovered = IsMouseInsideButtonNDC(CreditCenterX_NDC, CreditCenterY_NDC, CreditTextWidthNDC, CreditTextHeightNDC);
        f32 Credit_r = (CreditHovered) ? 0.5f : 1.0f;
        f32 Credit_g = (CreditHovered) ? 0.0f : 1.0f;
        f32 Credit_b = (CreditHovered) ? 0.0f : 1.0f;

        f32 CreditTextX_NDC = CreditCenterX_NDC - (CreditTextWidthNDC * 0.5f);
        f32 CreditTextY_NDC = CreditCenterY_NDC - (CreditTextHeightNDC * 0.5f);
        AEGfxPrint(buttonFont, "Credit", CreditTextX_NDC, CreditTextY_NDC, 1.0f, Credit_r, Credit_g, Credit_b, 1.0f);
    }

    // ==============================================================
    // Color Change on Hover 'Setting' Button
    // ==============================================================    
    {
        f32 SettingTextWidthNDC, SettingTextHeightNDC;
        AEGfxGetPrintSize(buttonFont, "Setting", 1.0f, &SettingTextWidthNDC, &SettingTextHeightNDC);

        //f32 SettingCenterX_NDC = SettingRightEdgeNDC - (SettingTextWidthNDC * 0.5f);
        f32 SettingCenterX_NDC = 0.0f;
        f32 SettingCenterY_NDC = -0.6f;

        bool SettingHovered = IsMouseInsideButtonNDC(SettingCenterX_NDC, SettingCenterY_NDC, SettingTextWidthNDC, SettingTextHeightNDC);
        f32 Setting_r = (SettingHovered) ? 0.5f : 1.0f;
        f32 Setting_g = (SettingHovered) ? 0.0f : 1.0f;
        f32 Setting_b = (SettingHovered) ? 0.0f : 1.0f;

        f32 SettingTextX_NDC = SettingCenterX_NDC - (SettingTextWidthNDC * 0.5f);
        f32 SettingTextY_NDC = SettingCenterY_NDC - (SettingTextHeightNDC * 0.5f);
        AEGfxPrint(buttonFont, "Setting", SettingTextX_NDC, SettingTextY_NDC, 1.0f, Setting_r, Setting_g, Setting_b, 1.0f);
    }

    // ==============================================================
    // Color Change on Hover 'Exit Game' Button
    // ==============================================================    
    {
        f32 ExitTextWidthNDC, ExitTextHeightNDC;
        AEGfxGetPrintSize(buttonFont, "Exit Game", 1.0f, &ExitTextWidthNDC, &ExitTextHeightNDC);

        f32 ExitCenterX_NDC = 0.0f;
        f32 ExitCenterY_NDC = -0.9f;

        bool ExitHovered = IsMouseInsideButtonNDC(ExitCenterX_NDC, ExitCenterY_NDC, ExitTextWidthNDC, ExitTextHeightNDC);
        f32 Exit_r = (ExitHovered) ? 0.5f : 1.0f;
        f32 Exit_g = (ExitHovered) ? 0.0f : 1.0f;
        f32 Exit_b = (ExitHovered) ? 0.0f : 1.0f;

        f32 ExitTextX_NDC = ExitCenterX_NDC - (ExitTextWidthNDC * 0.5f);
        f32 ExitTextY_NDC = ExitCenterY_NDC - (ExitTextHeightNDC * 0.5f);
        AEGfxPrint(buttonFont, "Exit Game", ExitTextX_NDC, ExitTextY_NDC, 1.0f, Exit_r, Exit_g, Exit_b, 1.0f);
    }
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
    if (creditButtonMesh) {
        AEGfxMeshFree(creditButtonMesh);
        creditButtonMesh = nullptr;
    }
    if (settingButtonMesh) {
        AEGfxMeshFree(settingButtonMesh);
        settingButtonMesh = nullptr;
    }
    if (exitButtonMesh) {
        AEGfxMeshFree(exitButtonMesh);
        exitButtonMesh = nullptr;
    }
}

void MainMenuState::Unload() {
    /*if (menuFont != -1) {
        AEGfxDestroyFont(menuFont);
        menuFont = -1;
    }
    if (buttonFont != -1) {
        AEGfxDestroyFont(buttonFont);
        buttonFont = -1;
    }*/
    AudioManager::UnloadAudio("MainMenu");

}

BaseState* MainMenuState::GetNextState() {
    if (startGame) {
        return new LevelSelectState();
        //return new GameState1();
        //return new gamePreview();
    }
    if (menu) {
        return new instructionState();
    }
    return nullptr;
}