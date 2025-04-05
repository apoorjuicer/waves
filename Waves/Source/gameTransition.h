/* Start Header ************************************************************************/
/*!
\file		gameTransition.h
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

#pragma once
#include "pch.h"

#include "mainMenu.h"
#include "game1.h"
#include "game2.h"
#include "Player.h"

extern AEVec2 GlobalCenter;

class gameTransition : public BaseState {
private:
    bool continueGame = false;      // Flag for going to next Game
    bool quitGame = false;          // Flag for returning to the main menu
    s8 transitionFont;
    AEGfxVertexList* continueButtonMesh = nullptr;
    AEGfxVertexList* quitButtonMesh = nullptr;
    BaseState* nNextState;          // Pointer to the next state 
public:
    gameTransition(BaseState* nextState);
    void Load() override;
    void Initialize() override;
    void Update(f32 deltaTime) override;
    void Draw() override;
    void Free() override;
    void Unload() override;
    BaseState* GetNextState() override;
};
