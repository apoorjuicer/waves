/* Start Header ************************************************************************/
/*!
\file		game2.cpp
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

#include "game2.h"

namespace {
    Enemy enemy2;
}

GameState2::GameState2() : exitGame(false), winGame(false), tilesize(100.0f),
pDonutMesh(nullptr), PlayerMesh(nullptr), wallMesh(nullptr),
pathMesh(nullptr), enemyMesh(nullptr), rockMesh(nullptr), keyMesh(nullptr), 
goalMesh(nullptr), boxMesh(nullptr), keyTexture(nullptr), rockTexture(nullptr), playerTexture(nullptr), prevDestX(-1), prevDestY(-1), gameOverSoundPlayed(false),
gameWinSoundPlayed(false) {}

void GameState2::Load() {
    ClearMazeData(); // Ensure old maze is wiped

    worldItems.clear();
    if (!ImportMapDataFromFile("Assets/Maze2.txt")) {
        std::cerr << "Error: Failed to load maze file Assets/Maze1.txt. Quitting game.\n";
    }

    AEGfxSetBackgroundColor(0.1f, 0.1f, 0.1f);

    pDonutMesh = CreateDonutMesh(1000.0f, 100.0f, 48, 0xFF000000);
    PlayerMesh = Player_Mesh(48, 0xFFFFFFFF, 10.0f);
    wallMesh = CreateSquareMesh(tilesize, 0xFF000000);
    pathMesh = CreateSquareMesh(tilesize, 0x55555555);
    enemyMesh = createEnemyMesh(0xFFFF0000);
    goalMesh = CreateSquareMesh(tilesize, 0x5500FF00);
    keyMesh = CreateSquareMesh(30.0f, 0xFFFFD700);
    rockMesh = CreateSquareMesh(30.0f, 0x8B4513FF);
    boxMesh = CreateBoxMesh(0xFFFFFFFF);
    this->keyTexture = AEGfxTextureLoad("Assets/key.png");
    this->rockTexture = AEGfxTextureLoad("Assets/PlanetTexture.png");
    this->playerTexture = AEGfxTextureLoad("Assets/character_malePerson_idle.png");

    wave_meshes[0] = CreateWaveMesh(1.0f, 0.95f, 36);
    wave_meshes[1] = Create_SemiCircle_Mesh(1.0f, 0.98f, 36, 4);
    wave_meshes[2] = Create_LineMesh();

    createWorldItems(tilesize);
    //setEnemyPatrol(enemy2, maze);
    enemy2.setEnemyPatrol(maze);
}

void GameState2::Initialize() {
    exitGame = false;
    dead = false;
    winGame = false;
    player.x = 100.f;
    player.y = -100.f;
    player.inventory[0] = ROCK;

    //enemy2.x = 100.f;
    //enemy2.y = -700.f;
    //enemy2.startPatrol = { 2,7 };
    //enemy2.endPatrol = { 10,10 };
    //enemy2.x = enemy2.startPatrol.first * TILE_SIZE;
    //enemy2.y = enemy2.startPatrol.second * -TILE_SIZE;

    enemy2.startPatrol = enemy2.patrolPath.front();
    enemy2.endPatrol = enemy2.patrolPath.back();
    enemy2.x = enemy2.startPatrol.first * TILE_SIZE;
    enemy2.y = enemy2.startPatrol.second * -TILE_SIZE;
    enemy2.state = PATROL;
    enemy2.previousState = PATROL;
    //enemy2.idle_timer = 3.f;
    enemy2.patrol_timer = 20.f;
    //enemy2.pursuit_timer = 3.f;
    enemy2.updateGridPosition(); // need to call to init conversion
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

void GameState2::Update(f32 deltaTime) {
    if (!AudioManager::IsAudioPlaying("GameBGM")) {
        AudioManager::PlayAudio("GameBGM", 0.3f, 1.0f, -1);
    }
    // Toggle pause when ESC is pressed if not already paused
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

    AEGfxSetBackgroundColor(0.1f, 0.1f, 0.1f);

    // Player movement
    f32 speed = 200.0f;
    if (AEInputCheckCurr(AEVK_W)) MovePlayer(player, 0.0f, 1.0f, speed, deltaTime, maze, tilesize);
    if (AEInputCheckCurr(AEVK_S)) MovePlayer(player, 0.0f, -1.0f, speed, deltaTime, maze, tilesize);
    if (AEInputCheckCurr(AEVK_A)) MovePlayer(player, -1.0f, 0.0f, speed, deltaTime, maze, tilesize);
    if (AEInputCheckCurr(AEVK_D)) MovePlayer(player, 1.0f, 0.0f, speed, deltaTime, maze, tilesize);


    if (AEInputCheckTriggered(AEVK_SPACE)) {
        // spawn a directional wave towards player mouse;
        spawn_dir_wave(dir_wave, AEVec2{ player.x, player.y }, wave_meshes);
        AudioManager::PlayAudio("Waves", 1.0f, 1.0f, 0); // Play wave sound once
    }

    //debug
   /* if (AEInputCheckTriggered(AEVK_SPACE)) {
        spawn_wave(enemy_wave, enemy1.x, enemy1.y, wave_meshes);
    }*/

    enemy2.UpdateEnemystate(player, deltaTime, enemy_wave, wave_meshes, dir_wave);
    //enemy1.enemy_wave(enemy_wave, wave_meshes);

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

    CheckItem(player, maze, tilesize, 0.0f, 0);
    dead = PlayerEnemyCollision(player, {enemy2.x, enemy2.y}, tilesize);
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

void GameState2::Draw() {
    AEGfxSetCamPosition(player.x, player.y);
    if (!PauseOverlay::IsPaused()) {
        rock::drawRock(player, maze, tilesize, 0.0f, 0.0f, wave, wave_meshes, rockMesh, rockTexture);
    }
    RenderMesh(enemyMesh, enemy2.x, enemy2.y, enemy2.w, enemy2.h, 0.0f);

    RenderMaze(maze, tilesize, wallMesh, pathMesh, goalMesh, keyMesh, rockMesh, keyTexture, rockTexture);
    RenderMesh(PlayerMesh, player.x, player.y, 1.0f, 1.0f, 45.0f);

    //RenderMesh(pDonutMesh, player.x, player.y, 1.0f, 1.0f, 45.0f);

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
    // ---------------------------------------------------
    RenderPlayer(player, PlayerMesh, playerTexture);

    DrawInventory(player, boxMesh, keyMesh, rockMesh, keyTexture, rockTexture);

    // If paused, overlay the pause menu on top
    if (PauseOverlay::IsPaused()) {
        PauseOverlay::Draw();
    }
}

void GameState2::Free() {
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
    AEGfxTextureUnload(keyTexture);
    AEGfxTextureUnload(rockTexture);
    worldItems.clear();

    for (int i{};i < INVENTORY_SIZE;++i) {
        player.inventory[i] = NOTHING;
    }
    PauseOverlay::Free();
}

void GameState2::Unload() {
    AudioManager::UnloadAudio("GameBGM");
    AudioManager::UnloadAudio("Waves");
    AudioManager::UnloadAudio("Rock Throw");
    AudioManager::UnloadAudio("Collide");
    //AudioManager::UnloadAudio("GameOver");
    //AudioManager::UnloadAudio("Win");
	AudioManager::UnloadAudio("Collect");
}

BaseState* GameState2::GetNextState() {
    if (exitGame) {
        return new MainMenuState();
    }
    if (dead) {
        return new LoseState();
    }
    if (winGame) {
        return new gameTransition(new gamePreview(3));
    }
    return nullptr;
}
