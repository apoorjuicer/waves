#pragma once
#include "baseStates.h"
#include "AEEngine.h"
#include "game.h"

class MainMenuState : public BaseState {
private:
    bool startGame = false;
    bool quit = false;
    s8 menuFont;
public:
    MainMenuState();
    void Load() override;
    void Initialize() override;
    void Update(f32 deltaTime) override;
    void Draw() override;
    void Free() override;
    void Unload() override;
    BaseState* GetNextState() override;
};
