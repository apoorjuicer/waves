#pragma once
#include "baseStates.h"
#include "AEEngine.h"

class WinState : public BaseState {
private:
    bool goNext;       // Flag to trigger transition
    s8 winFont;
public:
    WinState();
    void Load() override;
    void Initialize() override;
    void Update(f32 deltaTime) override;
    void Draw() override;
    void Free() override;
    void Unload() override;
    BaseState* GetNextState() override;
};
