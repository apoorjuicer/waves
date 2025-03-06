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
