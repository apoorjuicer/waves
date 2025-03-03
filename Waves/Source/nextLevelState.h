#pragma once
#include "baseStates.h"

class NextLevelState : public BaseState {
private:
    bool loadNextGame;
public:
    NextLevelState();
    void Load() override;
    void Initialize() override;
    void Update(float deltaTime) override;
    void Draw() override;
    void Free() override;
    void Unload() override;
    BaseState* GetNextState() override;
};
