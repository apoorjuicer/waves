/* Start Header ************************************************************************/
/*!
\file		GSM.cpp
\project    Waves

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

#include "mainMenu.h"
#include "GSM.h"

// Constructor
GameStateManager::GameStateManager() {}

// Set the pointer to the game running flag
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

// Update the current state
void GameStateManager::Update(f32 deltaTime) {
    if (!currentState) return;

    currentState->Update(deltaTime);

    BaseState* nextState = currentState->GetNextState();
    if (nextState != nullptr) {
        SetState(nextState);    
    }
}

// Draw the current state
void GameStateManager::Draw() {
    if (currentState) {
        currentState->Draw();
    }
}

// Destructor
GameStateManager::~GameStateManager() {
    if (currentState) {
        currentState->Free();
        currentState->Unload();
        delete currentState;
    }
}
