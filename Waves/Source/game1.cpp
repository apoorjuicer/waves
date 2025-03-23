/* Start Header ************************************************************************/
/*!
\file		game1.cpp
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

#include "game1.h"
bool waveSpawned = false;  // Flag to track if the wave has been spawned
std::vector<WorldItem> worldItems;

namespace {
    Enemy enemy1;
}

GameState1::GameState1() : exitGame(false), winGame(false), tilesize(100.0f),
pDonutMesh(nullptr), PlayerMesh(nullptr), wallMesh(nullptr),
pathMesh(nullptr), enemyMesh(nullptr), rockMesh(nullptr), keyMesh(nullptr), 
goalMesh(nullptr), boxMesh(nullptr), prevDestX(-1), prevDestY(-1){}

void GameState1::Load() {
    worldItems.clear();
    if (!ImportMapDataFromFile("Assets/Maze1.txt")) {
        std::cerr << "Error: Failed to load maze file Assets/Maze1.txt. Quitting game.\n";
        // Transition to quit state, etc.
    }
    //Import_MapData();

    AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

    pDonutMesh = CreateDonutMesh(1000.0f, 100.0f, 48, 0xFF000000);
    PlayerMesh = Player_Mesh(48, 0xFFFFFFFF, 10.0f);
    wallMesh = CreateSquareMesh(tilesize, 0xFF000000);
    pathMesh = CreateSquareMesh(tilesize, 0x55555555);
    enemyMesh = createEnemyMesh(0xFFFF0000);
    goalMesh = CreateSquareMesh(tilesize, 0x5500FF00);
    keyMesh = rock::Rock_Mesh(48, 0xFFFFD700, 10.0f);
    rockMesh = rock::Rock_Mesh(48, 0x8B4513FF, 10.0f);
    boxMesh = CreateBoxMesh(0xFFFFFFFF);

    wave_meshes[0] = CreateWaveMesh(1.0f, 0.95f, 36);
    wave_meshes[1] = Create_SemiCircle_Mesh(1.0f, 0.98f, 36, 2);
    wave_meshes[2] = Create_LineMesh();
    createWorldItems(tilesize);

}

void GameState1::Initialize() {
    exitGame = false;
    dead = false;
    winGame = false;
    player.x = 100.f;
    player.y = -100.f;
    
    player.inventory[0] = ROCK;
    enemy1.x = 100.f;
    enemy1.y = -700.f;
    enemy1.startPatrol = { 1,7 };
    enemy1.endPatrol = { 8,7 };
   // enemy1.state = PATROL;
    enemy1.state = IDLE;
    //enemy1.state = IDLE;
   // enemy1.state = PURSUIT_ROCK;
    
    enemy1.patrolPath = { {1,7},{2,7},{3,7},{4,7},{5,7},{6,7},{7,7},{8,7} };
    enemy1.updateGridPosition(); // need to call to init conversion
    AEAudioGroup InGame = AEAudioCreateGroup();  // For background music
	AEAudioGroup RockThrow = AEAudioCreateGroup();  // Diff Grp to cancel sound
    AudioManager::LoadAudio("GameBGM", "Assets/Audio/GameBGM.mp3", true, InGame);
    AudioManager::LoadAudio("Waves", "Assets/Audio/SonarWave.wav", false, InGame);
    AudioManager::LoadAudio("Rock Throw", "Assets/Audio/Throw.wav", false, RockThrow);
    AudioManager::LoadAudio("Collide", "Assets/Audio/Collision.wav", false, InGame);
    AudioManager::LoadAudio("GameOver", "Assets/Audio/GameOver.wav", false, InGame);
    AudioManager::LoadAudio("Win", "Assets/Audio/Win.wav", false, InGame);
    AudioManager::PlayAudio("GameBGM", 0.3f, 1.0f, -1);
    PauseOverlay::Init();

}

void GameState1::Update(f32 deltaTime) {
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

    AEGfxSetBackgroundColor(0.f, 0.f, 0.f);

    // Player movement
    f32 speed = 200.0f;
    if (AEInputCheckCurr(AEVK_W)) MovePlayer(player, 0.0f, 1.0f, speed, deltaTime, maze, tilesize);
    if (AEInputCheckCurr(AEVK_S)) MovePlayer(player, 0.0f, -1.0f, speed, deltaTime, maze, tilesize);
    if (AEInputCheckCurr(AEVK_A)) MovePlayer(player, -1.0f, 0.0f, speed, deltaTime, maze, tilesize);
    if (AEInputCheckCurr(AEVK_D)) MovePlayer(player, 1.0f, 0.0f, speed, deltaTime, maze, tilesize);

    if (AEInputCheckTriggered(AEVK_SPACE) && !waveSpawned) {
        spawn_wave(wave, player.x, player.y, wave_meshes);
        AudioManager::PlayAudio("Waves", 1.0f, 1.0f, 0); // Play wave sound once
        waveSpawned = true; // Set flag to prevent further triggers
    }

    // Reset the flag when SPACE is released
    if (!AEInputCheckCurr(AEVK_SPACE)) {
        waveSpawned = false;
    }
    //debug
   /* if (AEInputCheckTriggered(AEVK_SPACE)) {
        spawn_wave(enemy_wave, enemy1.x, enemy1.y, wave_meshes);
    }*/

    UpdateEnemystate(enemy1, deltaTime, enemy_wave,wave_meshes);
    //enemy1.enemy_wave(enemy_wave, wave_meshes);

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

    CheckItem(player, maze, tilesize, 0.0f, 0);

    dead = PlayerEnemyCollision(player, enemy1, tilesize);
    //if (dead)
    //{
    //    AudioManager::PlayAudio("GameOver", 1.0f, 1.0f, -1);

    //}

    winGame = checkGoal(player, maze, tilesize, 0.0f, 0.0f);
    //if (winGame)
    //{
    //    AudioManager::PlayAudio("Win", 3.0f, 1.0f, -1);
    //}
}

void GameState1::Draw() {
    AEGfxSetCamPosition(player.x, player.y);
    rock::drawRock(player, maze, tilesize, 0.0f, 0.0f, wave, wave_meshes, lastRock);
    RenderMesh(enemyMesh, enemy1.x, enemy1.y, enemy1.w, enemy1.h, 0.0f);

    RenderMaze(maze, tilesize, wallMesh, pathMesh, goalMesh, keyMesh, rockMesh);
    RenderMesh(PlayerMesh, player.x, player.y, 1.0f, 1.0f, 45.0f);
    //(enemyMesh, enemy1.x, enemy1.y, enemy1.w, enemy1.h, 0.0f);

    RenderMesh(pDonutMesh, player.x, player.y, 1.0f, 1.0f, 45.0f);
    
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
    //for (Wave& r : reflections) {
    //    if (r.is_active) {
    //        r.draw();
    //    }
    //}
    for (Outline& l : outlines) {
        if (l.is_active) {
            l.draw();
        }
    }
    RenderPlayer(player, PlayerMesh);

    DrawInventory(player, boxMesh, keyMesh, rockMesh);

    // If paused, overlay the pause menu on top
    if (PauseOverlay::IsPaused()) {
        PauseOverlay::Draw();
    }
}

void GameState1::Free() {
    for (AEGfxVertexList* (&i) : wave_meshes) {
        if (i) {
            AEGfxMeshFree(i);
        }
    }

    AEGfxMeshFree(pDonutMesh);
    AEGfxMeshFree(PlayerMesh);
    AEGfxMeshFree(wallMesh);
    AEGfxMeshFree(pathMesh);
    AEGfxMeshFree(enemyMesh);
    AEGfxMeshFree(goalMesh);
    AEGfxMeshFree(keyMesh);
    AEGfxMeshFree(rockMesh);
    AEGfxMeshFree(boxMesh);
    worldItems.clear();
    AudioManager::UnloadAudio("GameBGM");
    for (int i{};i < INVENTORY_SIZE;++i) {
        player.inventory[i] = NOTHING;
    }
    PauseOverlay::Free();
}

void GameState1::Unload() {}

BaseState* GameState1::GetNextState() {
    //return exitGame ? new MainMenuState() : nullptr;
    //return dead ? new LoseState() : nullptr;
    //return winGame ? new WinState() : nullptr;

    if (exitGame) {
        return new MainMenuState();
    }
    if (dead) {
        return new LoseState();
    }
    if (winGame) {
        return new gameTransition();
    }
    //if (winGame) {
    //    return new WinState();
    //}
    return nullptr;  // Stay in the current state if nothing changes
}
