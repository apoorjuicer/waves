#include "select.h"


const f32 ButtonW = 100.0f;
const f32 ButtonH = 100.0f;

const f32 gamebuttonY = 0.0f;

const f32 game1X = -300.0f;
const f32 game2X = 0.0f;
const f32 game3X = 300.0f;

const f32 backbuttonY = 400.0f;
const f32 backbuttonX = -750.0f;

LevelSelectState::LevelSelectState() :  game1(nullptr), game2(nullptr), game3(nullptr), back2main(nullptr), 
                                        selectFont(-1), 
                                        game1ButtonMesh(nullptr), game2ButtonMesh(nullptr), game3ButtonMesh(nullptr), back2mainButtonMesh(nullptr) {}

void LevelSelectState::Load() {
    AEGfxSetBackgroundColor(0.f, 0.f, 0.f);

    selectFont = AEGfxCreateFont("Assets/Fonts/Nunito/Nunito-MediumItalic.ttf", 100);

    game1ButtonMesh =       CreateButtonMesh(0xFFFFFFFF);
    game2ButtonMesh =       CreateButtonMesh(0xFFFFFFFF);
    game3ButtonMesh =       CreateButtonMesh(0xFFFFFFFF);
    back2mainButtonMesh =   CreateButtonMesh(0xFFFFFFFF);
}

void LevelSelectState::Initialize() {
    // Reset state data.
    game1 = false;
    game2 = false;
    game3 = false;
    back2main = false;
}

void LevelSelectState::Update(f32 deltaTime) 
{
    (f32)deltaTime;

    if (AEInputCheckTriggered(AEVK_ESCAPE)) {
        back2main = true;
    }

    if (AEInputCheckTriggered(AEVK_LBUTTON)) {
        if (IsMouseInsideButton(game1X, gamebuttonY, ButtonW, ButtonH)) {
            game1 = true;
        }
        else if (IsMouseInsideButton(game2X, gamebuttonY, ButtonW, ButtonH)) {
            game2 = true;
        }
        else if (IsMouseInsideButton(game3X, gamebuttonY, ButtonW, ButtonH)) {
            game3 = true;
        }
        else if (IsMouseInsideButton(backbuttonX, backbuttonY, ButtonW, ButtonH)) {
            back2main = true;
        }
    }
}

void LevelSelectState::Draw() 
{
    float textWidth, textHeight;

    // Get text dimensions (normalized units) for "Game Paused":
    AEGfxGetPrintSize(selectFont, "Levels", 1.0f, &textWidth, &textHeight);

    // Center the text horizontally by subtracting half its width,
    // and center vertically by subtracting half its height from 0.0f
    // so that it’s in the exact middle of the screen.
    float centerX = -textWidth * 0.5f;
    float centerY = textHeight * 0.5f + 0.5f;

    // Print at scale = 1.0, with white color (1.0f,1.0f,1.0f,1.0f).
    AEGfxPrint(selectFont, "Levels", centerX, centerY, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);


    // Draw button "1"
    DrawButton(game1ButtonMesh, game1X, gamebuttonY, "1", selectFont, ButtonW, ButtonH);

    // Draw button "2" 
    DrawButton(game2ButtonMesh, game2X, gamebuttonY, "2", selectFont, ButtonW, ButtonH);

    // Draw button "3"
    DrawButton(game3ButtonMesh, game3X, gamebuttonY, "3", selectFont, ButtonW, ButtonH);

    // Draw Exit to Menu
    DrawButton(back2mainButtonMesh, backbuttonX, backbuttonY, " ", selectFont, ButtonW, ButtonH);
}

void LevelSelectState::Free() {
    // Free any runtime-created resources if needed.
}

void LevelSelectState::Unload() {
    // Unload assets (textures, fonts) that were loaded in Load().
    // For example:
    // AEGfxTextureUnload(levelThumbnail);
    // AEGfxDestroyFont(myFontId);
}

BaseState* LevelSelectState::GetNextState() 
{
    if (game1) {
        return new GameState1();
    }
    if (game2) {
        return new GameState2();
    }
    if (game3) {
        return new GameState3();
    }
    if (back2main) {
        return new MainMenuState();
    }
    return nullptr;
}
