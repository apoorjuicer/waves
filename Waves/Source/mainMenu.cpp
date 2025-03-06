#include "AEEngine.h"
#include "mainMenu.h"
#include "Player.h"

MainMenuState::MainMenuState() : startGame(false), quit(false), menuFont(-1) {}

void MainMenuState::Load() {
    AEGfxSetBackgroundColor(0.3f, 0.3f, 0.3f);

    // Use relative path instead of absolute path
    menuFont = AEGfxCreateFont("Assets/liberation-mono.ttf", 32);

    if (menuFont == -1) {
        printf("ERROR: Failed to load font\n");
    }
}

void MainMenuState::Initialize() {
    AEInputReset();    // Reset any prior input events
    startGame = false;
}

void MainMenuState::Update(f32 deltaTime) {
    (void)deltaTime;
    if (AEInputCheckTriggered(AEVK_RETURN)) {
        startGame = true;
    }
}

void MainMenuState::Draw() {
    if (menuFont != -1) {
        AEGfxPrint(menuFont, "Wave", -0.05f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
        //AEGfxPrint(menuFont, "Press ENTER to start", -0.4f, 0.3f, 0.8f, 1.0f, 1.0f, 1.0f, 1.0f);
        //AEGfxPrint(menuFont, "Press ESC to quit", -0.35f, 0.1f, 0.8f, 1.0f, 1.0f, 1.0f, 1.0f);
    }
}

void MainMenuState::Free() {
}

void MainMenuState::Unload() {
    if (menuFont != -1) {
        AEGfxDestroyFont(menuFont);
        menuFont = -1;
    }
}

BaseState* MainMenuState::GetNextState() {
    return startGame ? new GameState() : nullptr;
}