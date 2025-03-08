#pragma once
#include "baseStates.h"
#include "AEEngine.h"

class instructionState : public BaseState {
private:
    bool goMenu;   // Flag for returning to the main menu
public:
    instructionState();
    void Load() override;
    void Initialize() override;
    void Update(f32 deltaTime) override;
    void Draw() override;
    void Free() override;
    void Unload() override;
    BaseState* GetNextState() override;
};
