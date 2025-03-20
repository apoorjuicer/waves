/* Start Header ************************************************************************/
/*!
\file		mainMenu.h
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

#pragma once
#include "baseStates.h"
#include "AEEngine.h"
#include "game1.h"
#include "select.h"
#include "gamePreview.h"
#include "instructions.h"
#include "Audio.h"
#include "Player.h"

class MainMenuState : public BaseState {
private:
    bool startGame = false;
    bool menu = false;
    int* pGameRunning; // <-- Pointer to gGameRunning
    s8 menuFont;
    s8 buttonFont;
    AEGfxVertexList* playButtonMesh = nullptr;  // Mesh for the 'Play' button
    AEGfxVertexList* guideButtonMesh = nullptr;  // Mesh for 'Guide' button
    AEGfxVertexList* exitButtonMesh = nullptr;   // Mesh for 'Exit' button
public:
    MainMenuState();

    // Let the caller inject a pointer to gGameRunning
    void SetGameRunningPtr(int* ptr) { pGameRunning = ptr; }

    void Load() override;
    void Initialize() override;
    void Update(f32 deltaTime) override;
    void Draw() override;
    void Free() override;
    void Unload() override;
    BaseState* GetNextState() override;
};

bool IsMouseInsideButton(float buttonX, float buttonY, float buttonWidth, float buttonHeight);
AEGfxVertexList* CreateButtonMesh(u32 color);
void DrawButton(AEGfxVertexList* buttonMesh, float x, float y, const char* text, s8 font, float scaleX, float scaleY);