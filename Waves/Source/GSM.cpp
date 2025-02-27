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
        currentState->Load();
        currentState->Initialize();
    }
    ignoreEsc = true;
}


void GameStateManager::Update(f32 deltaTime, int& gGameRunning) {
    if (!currentState) return;

    currentState->Update(deltaTime);

    BaseState* nextState = currentState->GetNextState();
    if (nextState != nullptr) {
        SetState(nextState);    
    }

    if (0 == AESysDoesWindowExist()) {
        gGameRunning = 0;
    }

    // If we are in ignore mode, check if the ESC key has been released
    if (ignoreEsc) {
        if (AEInputCheckReleased(AEVK_ESCAPE)) {
            ignoreEsc = false;
        }
    }

    // Only process ESC if we are not ignoring it.
    if (!ignoreEsc && AEInputCheckTriggered(AEVK_ESCAPE)) {
        // If in MainMenuState, quit the program.
        if (dynamic_cast<class MainMenuState*>(currentState)) {
            gGameRunning = 0;
        }
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
