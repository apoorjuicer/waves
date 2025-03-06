#include "winState.h"
#include "AEEngine.h"
#include <cstdio>

WinState::WinState() : goNext(false) {}

void WinState::Load() {
    // E.g. set background color or load victory assets
    AEGfxSetBackgroundColor(0.0f, 0.5f, 0.0f); // Dark green background
}

void WinState::Initialize() {
    goNext = false;
}

void WinState::Update(float deltaTime) {
    // Example: Press ENTER to proceed to next level
    if (AEInputCheckTriggered(AEVK_RETURN)) {
        goNext = true;
    }
}

void WinState::Draw() {
    // Show "You Win!" text
    // If you have a font, e.g.:
    // AEGfxPrint(someFontId, "You Win!", -0.2f, 0.0f, scale, r, g, b, a);
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
}
