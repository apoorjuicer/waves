#pragma once
#include "baseStates.h"
#include "AEEngine.h"
#include "game.h"

class MainMenuState : public BaseState {
private:
    bool startGame = false;
    int* pGameRunning; // <-- Pointer to gGameRunning
    s8 menuFont;
    s8 buttonFont;
    AEGfxVertexList* playButtonMesh = nullptr;  // Mesh for the 'Play' button
    AEGfxVertexList* guideButtonMesh = nullptr;  // Mesh for 'Guide' button
    AEGfxVertexList* exitButtonMesh = nullptr;   // Mesh for 'Exit' button
public:
    MainMenuState();

    // Let the caller inject a pointer to gGameRunning
    void SetGameRunningPtr(int* ptr) { pGameRunning = ptr; }

    void Load() override;
    void Initialize() override;
    void Update(f32 deltaTime) override;
    void Draw() override;
    void Free() override;
    void Unload() override;
    BaseState* GetNextState() override;
};
