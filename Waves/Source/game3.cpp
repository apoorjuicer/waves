/* Start Header ************************************************************************/
/*!
\file		game3.cpp
\author		Henry Hu
\co-author  Jordain Ng, Benjamin Ban, Chewn Thing Kwan, Guan Shao Jun
\email		zhihenghenrey.hu@digipen.edu

\brief      This file defines the GameState class, which manages the core gameplay logic and rendering.
            It handles game state transitions, updates,
            and rendering of various game elements such as the player, enemies, maze, and interactive objects.
            The class integrates multiple components to create a cohesive gameplay experience.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "game3.h"

namespace {
    Enemy enemy3;
}

GameState3::GameState3() : exitGame(false), winGame(false), tilesize(100.0f),
pDonutMesh(nullptr), PlayerMesh(nullptr), wallMesh(nullptr),
pathMesh(nullptr), enemyMesh(nullptr), rockMesh(nullptr), keyMesh(nullptr),
goalMesh(nullptr), boxMesh(nullptr), keyTexture(nullptr), rockTexture(nullptr), 
playerTexture(nullptr), pathTexture(nullptr), doorTexture(nullptr),  enemyTexture(nullptr),
prevDestX(-1), prevDestY(-1),gameOverSoundPlayed(false),
gameWinSoundPlayed(false) {}

void GameState3::Load() {
    ClearMazeData(); // Ensure old maze is wiped

    worldItems.clear();
    if (!ImportMapDataFromFile("Assets/Maze3.txt")) {
        std::cerr << "Error: Failed to load maze file Assets/Maze3.txt. Quitting game.\n";
    }
    
    player.texture = AEGfxTextureLoad("Assets/Textures/Texture_Player.png");
    player.mesh = CreateTextureMesh(4, 2);

    pDonutMesh = CreateDonutMesh(1000.0f, 100.0f, 48, 0xFF000000);
    wallMesh = CreateSquareMesh(tilesize, 0xFF000000);
    pathMesh = CreateTextureMesh(1, 1);
    goalMesh = CreateTextureMesh(1, 1);
    enemyMesh = CreateTextureMesh(1, 1);
    keyMesh = CreateSquareMesh(30.0f, 0xFFFFD700);
    rockMesh = CreateSquareMesh(50.0f, 0x8B4513FF);
    boxMesh = CreateBoxMesh(0xFFFFFFFF);
    this->keyTexture = AEGfxTextureLoad("Assets/key.png");
    this->rockTexture = AEGfxTextureLoad("Assets/rock.png");
    this->playerTexture = AEGfxTextureLoad("Assets/character_malePerson_idle.png");
    this->pathTexture = AEGfxTextureLoad("Assets/Textures/Texture_Path2.png");
    this->doorTexture = AEGfxTextureLoad("Assets/Textures/Texture_Door.png");
    this->enemyTexture = AEGfxTextureLoad("Assets/Textures/Texture_Enemy.png");

    wave_meshes[0] = CreateWaveMesh(1.0f, 0.95f, 36);
    wave_meshes[1] = Create_SemiCircle_Mesh(1.0f, 0.98f, 36, 4);
    wave_meshes[2] = Create_LineMesh();
    createWorldItems(tilesize);


    enemy3.setEnemyPatrol(maze);
}

void GameState3::Initialize() {
    exitGame = false;
    dead = false;
    winGame = false;
    player.x = 100.f;
    player.y = -100.f;

    player.inventory[0] = ROCK;
    //enemy3.x = 100.f;
    //enemy3.y = -700.f;
    //enemy3.startPatrol = { 1,7 };
    //enemy3.endPatrol = { 8,7 };

    enemy3.startPatrol = enemy3.patrolPath.front();
    enemy3.endPatrol = enemy3.patrolPath.back();
    enemy3.x = enemy3.startPatrol.first * TILE_SIZE;
    enemy3.y = enemy3.startPatrol.second * -TILE_SIZE;

    // enemy1.state = PATROL;
    enemy3.state = PATROL;
    enemy3.previousState = PATROL;
    //enemy1.state = IDLE;
   // enemy1.state = PURSUIT_ROCK;

   // enemy3.patrolPath = { {1,7},{2,7},{3,7},{4,7},{5,7},{6,7},{7,7},{8,7} };
    enemy3.updateGridPosition(); // need to call to init conversion
    AEAudioGroup InGame = AEAudioCreateGroup();  // For background music
    AEAudioGroup RockThrow = AEAudioCreateGroup();  // Diff Grp to cancel sound
    AudioManager::LoadAudio("GameBGM", "Assets/Audio/GameBGM.mp3", true, InGame);
    AudioManager::LoadAudio("Waves", "Assets/Audio/SonarWave.wav", false, InGame);
    AudioManager::LoadAudio("Rock Throw", "Assets/Audio/Throw.wav", false, RockThrow);
    AudioManager::LoadAudio("Collide", "Assets/Audio/Collision.wav", false, InGame);
    //AudioManager::LoadAudio("GameOver", "Assets/Audio/GameOver.wav", false, InGame);
    //AudioManager::LoadAudio("Win", "Assets/Audio/Win.wav", false, InGame);
    AudioManager::LoadAudio("Collect", "Assets/Audio/Collect.wav", false, InGame);
    AudioManager::PlayAudio("GameBGM", 0.3f, 1.0f, -1);
    PauseOverlay::Init();
}

void GameState3::Update(f32 deltaTime) {
    // Toggle pause when ESC is pressed if not already paused
    if (!AudioManager::IsAudioPlaying("GameBGM")) {
        AudioManager::PlayAudio("GameBGM", 0.3f, 1.0f, -1);
    }
    if (AEInputCheckTriggered(AEVK_ESCAPE) && !PauseOverlay::IsPaused()) {
        PauseOverlay::SetPaused(true);
        AEInputReset();
    }
    // If paused, update the pause overlay and skip game updates
    if (PauseOverlay::IsPaused()) {
        PauseOverlay::Update();
        if (PauseOverlay::ShouldExitToMenu()) {
            exitGame = true;
        }
        return;
    }

    // Player movement
    f32 speed = 200.0f; player.state = player.PLAYER_STATE::IS_IDLE;
    if (AEInputCheckCurr(AEVK_W)) {
        player.state = player.PLAYER_STATE::IS_MOVING;
        MovePlayer(player, 0.0f, 1.0f, speed, deltaTime, maze, tilesize);
    }
    if (AEInputCheckCurr(AEVK_S)) {
        player.state = player.PLAYER_STATE::IS_MOVING;
        MovePlayer(player, 0.0f, -1.0f, speed, deltaTime, maze, tilesize);
    }
    if (AEInputCheckCurr(AEVK_A)) {
        player.state = player.PLAYER_STATE::IS_MOVING;
        MovePlayer(player, -1.0f, 0.0f, speed, deltaTime, maze, tilesize);
        player.dir = player.PLAYER_STATE::FACING_LEFT;
    }
    if (AEInputCheckCurr(AEVK_D)) {
        player.state = player.PLAYER_STATE::IS_MOVING;
        MovePlayer(player, 1.0f, 0.0f, speed, deltaTime, maze, tilesize);
        player.dir = player.PLAYER_STATE::FACING_RIGHT;
    }
    GlobalCenter.x = player.x; GlobalCenter.y = player.y;

    if (AEInputCheckTriggered(AEVK_SPACE) && !dir_wave.is_active) {
        // spawn a directional wave towards player mouse;
        spawn_dir_wave(dir_wave, AEVec2{ player.x, player.y }, wave_meshes);
        AudioManager::PlayAudio("Waves", 1.0f, 1.0f, 0); // Play wave sound once
    }

    enemy3.UpdateEnemystate(player,deltaTime, enemy_wave, wave_meshes, dir_wave);

    // UPDATE WAVES-----------------------------------------------------
    dir_wave.update();
    dir_wave.check_collide();
    dir_wave.echolocation(tilesize, outlines);
    for (Wave& w : wave) {
        if (w.is_active) {
            // update wave size;
            w.update();
            w.echolocation(tilesize, reflections, outlines);
        }
    }
    for (Wave& w : enemy_wave) {
        if (w.is_active) {
            w.update();

        }

    }
    for (Wave& r : reflections) {
        if (r.is_active) {
            r.update();
        }
    }
    for (Outline& l : outlines) {
        if (l.is_active) {
            l.update();
        }
    }
    // -----------------------------------------------------------------

    CheckItem(player);

    dead = PlayerEnemyCollision(player, { enemy3.x, enemy3.y }, tilesize);
    winGame = checkGoal(player, maze, tilesize, 0.0f, 0.0f);
    if (dead && !gameOverSoundPlayed)
    {
        AudioManager::PlayAudio("GameOver", 1.0f, 1.0f, 0);
        gameOverSoundPlayed = true;  // Ensure it doesn't play again
    }

    if (winGame && !gameWinSoundPlayed)
    {
        AudioManager::PlayAudio("Win", 1.0f, 1.0f, 0);
        gameWinSoundPlayed = true;  // Ensure it doesn't play again
    }
}


void GameState3::Draw() {
    AEGfxSetCamPosition(GlobalCenter.x, GlobalCenter.y);

    // render maze elements;
    RenderBackground(tilesize, pathMesh, pathTexture);
    RenderItems(keyMesh, rockMesh, keyTexture, rockTexture);
    // render enemy;
    DrawTexturedEntity(AEVec2{ enemy3.x, enemy3.y }, enemyTexture, enemyMesh);
    // render rock;
    if (!PauseOverlay::IsPaused()) {
        rock::drawRock(player, maze, tilesize, 0.0f, 0.0f, wave, wave_meshes, rockMesh, rockTexture);
	}
    // vision radius;
    RenderMesh(pDonutMesh, player.x, player.y, 1.0f, 1.0f, 45.0f);

    // DRAW WAVES-----------------------------------------
    dir_wave.draw();
    for (Wave& w : wave) {
        if (w.is_active) {
            w.draw();
        }
    }
    for (Wave& w : enemy_wave) {
        if (w.is_active) {
            w.draw();
        }
    }
    for (Outline& l : outlines) {
        if (l.is_active) {
            l.draw();
        }
    }
    // ---------------------------------------------------
    // render player;
    DrawPlayer(player);
    // render walls;
    RenderWalls(tilesize, wallMesh, goalMesh, doorTexture);

    DrawInventory(player, boxMesh, keyMesh, rockMesh, keyTexture, rockTexture);

    // If paused, overlay the pause menu on top
    if (PauseOverlay::IsPaused()) {
        PauseOverlay::Draw();
    }
}

void GameState3::Free() {
    for (AEGfxVertexList* (&i) : wave_meshes) {
        if (i) {
            AEGfxMeshFree(i);
        }
    }

    // free player
    AEGfxMeshFree(player.mesh);
    AEGfxTextureUnload(player.texture);

    AEGfxMeshFree(pDonutMesh);
    AEGfxMeshFree(wallMesh);
    AEGfxMeshFree(pathMesh);
    AEGfxMeshFree(enemyMesh);
    AEGfxMeshFree(goalMesh);
    AEGfxMeshFree(keyMesh);
    AEGfxMeshFree(rockMesh);
    AEGfxMeshFree(boxMesh);
    AEGfxTextureUnload(keyTexture);
    AEGfxTextureUnload(rockTexture);
    AEGfxTextureUnload(pathTexture);
    AEGfxTextureUnload(doorTexture);
    AEGfxTextureUnload(enemyTexture);
    worldItems.clear();

    for (int i{};i < INVENTORY_SIZE;++i) {
        player.inventory[i] = NOTHING;
    }
    PauseOverlay::Free();
}

void GameState3::Unload() {
    AudioManager::UnloadAudio("GameBGM");
    AudioManager::UnloadAudio("Waves");
    AudioManager::UnloadAudio("Rock Throw");
    AudioManager::UnloadAudio("Collide");
    //AudioManager::UnloadAudio("GameOver");
    //AudioManager::UnloadAudio("Win");
	AudioManager::UnloadAudio("Collect");
}

BaseState* GameState3::GetNextState() 
{
    if (exitGame) {
        return new MainMenuState();
    }
    if (dead) {
        return new LoseState();
    }
    if (winGame) {
        return new WinState();
    }
    return nullptr;
}
