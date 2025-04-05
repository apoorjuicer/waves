/* Start Header ************************************************************************/
/*!
\file		splash.h
\author		Henrey Hu
\email		zhihenghenrey.hu@digipen.edu

\brief      This file defines the SplashState class, which manages the splash screen of the game.
            It handles loading, rendering, and transitioning to the main menu after a specified duration.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

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
