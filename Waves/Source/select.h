/* Start Header ************************************************************************/
/*!
\file		select.h
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

#pragma once

#include "pch.h"
#include "mainMenu.h"
#include "game1.h"
#include "game2.h"
#include "game3.h"

class LevelSelectState : public BaseState {
private:
    bool game1 = false;      
    bool game2 = false;     
    bool game3 = false;
    bool back2main = false;
    s8 selectFont;
    AEGfxVertexList* game1ButtonMesh = nullptr;
    AEGfxVertexList* game2ButtonMesh = nullptr;
    AEGfxVertexList* game3ButtonMesh = nullptr;
    AEGfxVertexList* back2mainButtonMesh = nullptr;
	AEGfxTexture* pHomeIconTexture = nullptr;

public:
    LevelSelectState();
    void Load() override;                // Load assets (e.g. fonts, textures)
    void Initialize() override;          // Setup initial state data
    void Update(f32 deltaTime) override; // Process input and logic
    void Draw() override;                // Render UI elements
    void Free() override;                // Free runtime resources
    void Unload() override;              // Unload assets
    BaseState* GetNextState() override;  // Return pointer to the next state (if changed)
};


