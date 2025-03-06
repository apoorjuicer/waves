#include "loseState.h"
#include "mainMenu.h"
#include "AEEngine.h"

LoseState::LoseState() : goMenu(false) {}

void LoseState::Load() {
    AEGfxSetBackgroundColor(0.5f, 0.0f, 0.0f); // Dark red background
}

void LoseState::Initialize() {
    goMenu = false;
}

void LoseState::Update(float deltaTime) {
    // Example: Press ENTER to return to main menu
    if (AEInputCheckTriggered(AEVK_RETURN)) {
        goMenu = true;
    }
}

void LoseState::Draw() {
    // Show "You Lose!" text
    // AEGfxPrint(someFontId, "You Lose!", -0.2f, 0.0f, scale, 1.0f, 1.0f, 1.0f, 1.0f);
}

void LoseState::Free() {}

void LoseState::Unload() {}

BaseState* LoseState::GetNextState() {
    // If goMenu is true, transition to main menu
    return goMenu ? new MainMenuState() : nullptr;
}
