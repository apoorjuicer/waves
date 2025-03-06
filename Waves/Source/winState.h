#pragma once
#include "baseStates.h"

class WinState : public BaseState {
private:
    bool goNext;       // Flag to trigger transition
public:
    WinState();
    void Load() override;
    void Initialize() override;
    void Update(float deltaTime) override;
    void Draw() override;
    void Free() override;
    void Unload() override;
    BaseState* GetNextState() override;
};

