/* Start Header ************************************************************************/
/*!
\file		GSM.cpp
\author		Henrey Hu
\email		zhihenghenrey.hu@digipen.edu

\brief      This file defines the GameStateManager class, 
            which handles transitions between different game states. 
            It manages the current state, updates the game loop, and ensures 
            proper rendering and cleanup of states, maintaining smooth state transitions.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "AEEngine.h"
#include "mainMenu.h"
#include "GSM.h"
#include <iostream>

GameStateManager::GameStateManager() {}

void GameStateManager::SetState(BaseState* newState) {
    if (currentState) {
        currentState->Free();
        currentState->Unload();
        delete currentState;
    }

    currentState = newState;
    if (currentState) {
        if (auto mm = dynamic_cast<MainMenuState*>(currentState)) {
            mm->SetGameRunningPtr(pGameRunning);
        }

        currentState->Load();
        currentState->Initialize();
    }
}


void GameStateManager::Update(f32 deltaTime) {
    if (!currentState) return;

    currentState->Update(deltaTime);

    BaseState* nextState = currentState->GetNextState();
    if (nextState != nullptr) {
        SetState(nextState);    
    }
}

void GameStateManager::Draw() {
    if (currentState) {
        currentState->Draw();
    }
}

GameStateManager::~GameStateManager() {
    if (currentState) {
        currentState->Free();
        currentState->Unload();
        delete currentState;
    }
}
