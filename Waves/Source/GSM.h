#pragma once
#include "baseStates.h"

class GameStateManager {
private:
    BaseState* currentState = nullptr;
    int* pGameRunning = nullptr;
public:
    GameStateManager();

    void SetGameRunningPtr(int* ptr) { pGameRunning = ptr; }

    void SetState(BaseState* newState);
    void Update(f32 deltaTime);
    void Draw();
    ~GameStateManager();
};

