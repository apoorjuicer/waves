#include "nextLevelState.h"
#include "game.h"
#include "AEEngine.h"

NextLevelState::NextLevelState() : loadNextGame(false) {}

void NextLevelState::Load() {
    // Possibly load "Next Level" text or background
    AEGfxSetBackgroundColor(0.0f, 0.0f, 0.3f); // Dark blue
}

void NextLevelState::Initialize() {
    loadNextGame = false;
}

void NextLevelState::Update(float deltaTime) {
    // Press ENTER to load the next game stage
    if (AEInputCheckTriggered(AEVK_RETURN)) {
        loadNextGame = true;
    }
}

void NextLevelState::Draw() {
    // E.g. "Preparing Next Level..." text
    // AEGfxPrint(fontId, "Preparing Next Level...", -0.3f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
}

void NextLevelState::Free() {}

void NextLevelState::Unload() {}

BaseState* NextLevelState::GetNextState() {
    // Return new GameState if user pressed ENTER
    return loadNextGame ? new GameState() : nullptr;
}
