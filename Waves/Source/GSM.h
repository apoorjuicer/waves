#pragma once
#include "baseStates.h"

class GameStateManager {
private:
    BaseState* currentState = nullptr;
    bool ignoreEsc = false;
public:
    GameStateManager();

    void SetState(BaseState* newState);
    void Update(f32 deltaTime, int& gGameRunning);
    void Draw();
    ~GameStateManager();
};

