#include "instructions.h"
#include "mainMenu.h"
#include "AEEngine.h"

instructionState::instructionState() : goMenu(false){}

void instructionState::Load() {
    AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

}

void instructionState::Initialize() {
    goMenu = false;
}

void instructionState::Update(f32 deltaTime) {
    (f32)deltaTime;

    // Example: Press ENTER to return to main menu
    if (AEInputCheckTriggered(AEVK_RETURN)) {
        goMenu = true;
    }
}

void instructionState::Draw() {}

void instructionState::Free() {}

void instructionState::Unload() {}

BaseState* instructionState::GetNextState() {
    // If goMenu is true, transition to main menu
    return goMenu ? new MainMenuState() : nullptr;
}