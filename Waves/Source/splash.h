#pragma once
#include "pch.h"
#include "mainMenu.h"    
#include "gamePreview.h"

// Forward declaration for the next state
class MainMenuState;

class SplashState : public BaseState {
public:
    SplashState();
    virtual ~SplashState();

    virtual void Load() override;
    virtual void Initialize() override;
    virtual void Update(f32 deltaTime) override;
    virtual void Draw() override;
    virtual void Free() override;
    virtual void Unload() override;
    virtual BaseState* GetNextState() override;

private:
    float mTimeElapsed;
    float mSplashDuration; // Duration to show the splash screen (in seconds)
    AEGfxTexture* digipen = nullptr;
    AEGfxVertexList* digipen_mesh = nullptr;
};
