#pragma once
#include "baseStates.h"
#include "AEEngine.h"

class LoseState : public BaseState {
private:
    bool goMenu;   // Flag for returning to the main menu
    s8 loseFont;
    //bool retryGame; Button to go back to the game
public:
    LoseState();
    void Load() override;
    void Initialize() override;
    void Update(f32 deltaTime) override;
    void Draw() override;
    void Free() override;
    void Unload() override;
    BaseState* GetNextState() override;
};
