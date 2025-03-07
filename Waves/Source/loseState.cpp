#include "loseState.h"
#include "mainMenu.h"
#include "AEEngine.h"

LoseState::LoseState() : goMenu(false), loseFont(-1) {}

void LoseState::Load() {
    AEGfxSetBackgroundColor(0.5f, 0.0f, 0.0f); // Dark red background

    loseFont = AEGfxCreateFont("Assets/buggy-font.ttf", 80);
}

void LoseState::Initialize() {
    goMenu = false;
}

void LoseState::Update(f32 deltaTime) {
    // Example: Press ENTER to return to main menu
    if (AEInputCheckTriggered(AEVK_RETURN)) {
        goMenu = true;
    }

    // Use bool var goMenu when player 'dies' to trigger go mainMenu in GetNextState()
}

void LoseState::Draw() {
    if (loseFont != -1) {
        AEGfxPrint(loseFont, "Game Over", -0.65f, 0.0f, 1.5f, 0.0f, 0.0f, 0.0f, 1.0f);
    }
}

void LoseState::Free() {}

void LoseState::Unload() {}

BaseState* LoseState::GetNextState() {
    // If goMenu is true, transition to main menu
    return goMenu ? new MainMenuState() : nullptr;
}