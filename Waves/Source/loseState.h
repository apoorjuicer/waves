#pragma once
#include "baseStates.h"

class LoseState : public BaseState {
private:
    bool goMenu;   // Flag for returning to the main menu
public:
    LoseState();
    void Load() override;
    void Initialize() override;
    void Update(float deltaTime) override;
    void Draw() override;
    void Free() override;
    void Unload() override;
    BaseState* GetNextState() override;
};

