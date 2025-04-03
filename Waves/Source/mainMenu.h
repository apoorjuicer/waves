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
#include "utility.h"
#include "settingsOverlay.h"

class MainMenuState : public BaseState {
private:
    bool startGame = false;
    bool guide = false;
	bool setting = false;
    int* pGameRunning; // <-- Pointer to gGameRunning
    s8 menuFont;
    s8 buttonFont;
    AEGfxVertexList* playButtonMesh = nullptr;      // Mesh for the 'Play' button
    AEGfxVertexList* guideButtonMesh = nullptr;     // Mesh for 'Guide' button
    AEGfxVertexList* creditButtonMesh = nullptr;    // Mesh for 'Credits' button
    AEGfxVertexList* settingButtonMesh = nullptr;   // Mesh for 'Settings' button
    AEGfxVertexList* exitButtonMesh = nullptr;      // Mesh for 'Exit' button
	bool waitingForMouseRelease = true; 		    // Prevents multiple clicks  
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
