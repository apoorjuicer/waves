#include "winState.h"
#include "mainMenu.h"
#include "AEEngine.h"

WinState::WinState() : goNext(false), winFont(-1) {}

void WinState::Load() {
    // E.g. set background color or load victory assets
    AEGfxSetBackgroundColor(0.0f, 0.5f, 0.0f); // Dark green background

    winFont = AEGfxCreateFont("Assets/buggy-font.ttf", 80);
}

void WinState::Initialize() {
    goNext = false;
}

void WinState::Update(f32 deltaTime) {
    // Example: Press ENTER to proceed to next level
    if (AEInputCheckTriggered(AEVK_RETURN)) {
        goNext = true;
    }

    // Use bool var goNext when player won to trigger currently *So go mainMenu in GetNextState()* as winState only shows when completes all the maps
}

void WinState::Draw() {
    if (winFont != -1) {
        AEGfxPrint(winFont, "You Win", -0.48f, 0.0f, 1.5f, 0.0f, 0.0f, 0.0f, 1.0f);
    }
}

void WinState::Free() {
    // Free any resources if needed
}

void WinState::Unload() {
    // Unload any assets if needed
}

BaseState* WinState::GetNextState() {
    // If goNext is true, transition to NextLevelState
    //return goNext ? new NextLevelState() : nullptr;
    return goNext ? new MainMenuState() : nullptr;
}