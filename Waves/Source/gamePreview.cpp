#include "gamePreview.h"
#include "mainMenu.h"
#include "AEEngine.h"

gamePreview::gamePreview() : goGame(false) {}

void gamePreview::Load() {
    AEGfxSetBackgroundColor(0.5f, 0.0f, 0.0f); // Dark red background
}

void gamePreview::Initialize() {
    goGame = false;
}

void gamePreview::Update(f32 deltaTime) {
    // Example: Press ENTER to return to main menu
    if (AEInputCheckTriggered(AEVK_RETURN)) {
        goGame = true;
    }
}

void gamePreview::Draw() {
    // Show "You Lose!" text
    // AEGfxPrint(someFontId, "You Lose!", -0.2f, 0.0f, scale, 1.0f, 1.0f, 1.0f, 1.0f);
}

void gamePreview::Free() {}

void gamePreview::Unload() {}

BaseState* gamePreview::GetNextState() {
    // If goMenu is true, transition to main menu
    return goGame ? new MainMenuState() : nullptr;
}