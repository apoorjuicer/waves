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

namespace{
    Enemy enemy1;
}

GameState1::GameState1() : exitGame(false), winGame(false), tilesize(100.0f),
pDonutMesh(nullptr), PlayerMesh(nullptr), wallMesh(nullptr),
pathMesh(nullptr), enemyMesh(nullptr), rockMesh(nullptr), keyMesh(nullptr), 
goalMesh(nullptr), boxMesh(nullptr), keyTexture(nullptr), rockTexture(nullptr), playerTexture(nullptr), prevDestX(-1), prevDestY(-1), gameOverSoundPlayed(false),
gameWinSoundPlayed(false)// Declare this outside the game loop
{}

void GameState1::Load() {
    ClearMazeData(); // Ensure old maze is wiped

    worldItems.clear();
    if (!ImportMapDataFromFile("Assets/Maze1.txt")) {
        std::cerr << "Error: Failed to load maze file Assets/Maze1.txt. Quitting game.\n";
    }

    AEGfxSetBackgroundColor(0.1f, 0.1f, 0.1f);

    pDonutMesh = CreateDonutMesh(1000.0f, 100.0f, 48, 0xFF000000);
    PlayerMesh = CreateSquareMesh(50.0f, 0xFFFFFFFF);
    wallMesh = CreateSquareMesh(tilesize, 0xFF000000);
    pathMesh = CreateSquareMesh(tilesize, 0x55555555);
    enemyMesh = createEnemyMesh(0xFFFF0000);
    goalMesh = CreateSquareMesh(tilesize, 0x5500FF00);
    keyMesh = CreateSquareMesh(30.0f, 0xFFFFD700);
    rockMesh = CreateSquareMesh(50.0f, 0x8B4513FF);
    boxMesh = CreateBoxMesh(0xFFFFFFFF);
    this->keyTexture = AEGfxTextureLoad("Assets/key.png");
    if (!this->keyTexture) {
        std::cerr << "Error: Failed to load key texture!" << std::endl;
    }
    this->rockTexture = AEGfxTextureLoad("Assets/rock.png");
    if (!this->rockTexture) {
        std::cerr << "Error: Failed to load rock texture!" << std::endl;
    }
    this->playerTexture = AEGfxTextureLoad("Assets/character_malePerson_idle.png");

    wave_meshes[0] = CreateWaveMesh(1.0f, 0.95f, 36);
    wave_meshes[1] = Create_SemiCircle_Mesh(1.0f, 0.98f, 36, 4);
    wave_meshes[2] = Create_LineMesh();
    createWorldItems(tilesize);

    //setEnemyPatrol(enemy,1maze);
    enemy1.setEnemyPatrol(maze);


    //debug print maze
  /*  for (int i = 0;i < MAX_ROW;i++) {
        for (int j = 0;j < MAX_COL;j++) {
            std::cout << maze[i][j] << ',';
        }
        std::cout << "\n";
    }*/

    //for (size_t i = 0; i < enemy1.patrolPath.size(); i++) {
    //    std::cout << "Patrol Point " << i << ": ("
    //        << enemy1.patrolPath[i].second << ", "
    //        << enemy1.patrolPath[i].first << ")\n";
    //}
}

void GameState1::Initialize() {
    exitGame = false;
    dead = false;
    winGame = false;
    player.x = 300.f;
    player.y = -100.f;
    
    player.inventory[0] = ROCK;
    //enemy1.x = 100.f;
    //enemy1.y = -700.f;
  
    enemy1.startPatrol = enemy1.patrolPath.front();
    enemy1.endPatrol = enemy1.patrolPath.back();
    enemy1.x = enemy1.startPatrol.first * TILE_SIZE;
    enemy1.y = enemy1.startPatrol.second * -TILE_SIZE;
    //enemy1.x = 1 * TILE_SIZE;
    //enemy1.y = 1 * -TILE_SIZE;
    //enemy1.patrolPath = { {1,7},{2,7},{3,7},{4,7},{5,7},{6,7},{7,7},{8,7} };
    enemy1.state = PURSUIT_PLAYER;
    enemy1.previousState = PURSUIT_PLAYER;
    enemy1.updateGridPosition(); // need to call to init conversion
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

void GameState1::Update(f32 deltaTime) {
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


    //debug
    //enemy1.print_debug();
   // std::cout << player.x << player.y <<"\n";
    if (AEInputCheckCurr(AEVK_1)) {
        enemy1.state = IDLE;
        std::cout << "state changed to :" << enemy1.GetEnemyStateName() << "\n";
    }
    if (AEInputCheckCurr(AEVK_2)) {
        enemy1.state = PATROL;
        std::cout << "state changed to :" << enemy1.GetEnemyStateName() << "\n";
    }
    if (AEInputCheckCurr(AEVK_3)) {
        enemy1.state = PURSUIT_PLAYER;
        std::cout << "state changed to :" << enemy1. GetEnemyStateName() << "\n";
    }
    if (AEInputCheckCurr(AEVK_4)) {
        enemy1.state = PURSUIT_ROCK;
        std::cout << "state changed to :" << enemy1.GetEnemyStateName() << "\n";
    }
    AEGfxSetBackgroundColor(0.1f, 0.1f, 0.1f);

    // Player movement
    f32 speed = 200.0f;
    if (AEInputCheckCurr(AEVK_W)) MovePlayer(player, 0.0f, 1.0f, speed, deltaTime, maze, tilesize);
    if (AEInputCheckCurr(AEVK_S)) MovePlayer(player, 0.0f, -1.0f, speed, deltaTime, maze, tilesize);
    if (AEInputCheckCurr(AEVK_A)) MovePlayer(player, -1.0f, 0.0f, speed, deltaTime, maze, tilesize);
    if (AEInputCheckCurr(AEVK_D)) MovePlayer(player, 1.0f, 0.0f, speed, deltaTime, maze, tilesize);

    if (AEInputCheckTriggered(AEVK_SPACE) && !waveSpawned) {
        // spawn a directional wave towards player mouse;
        spawn_dir_wave(dir_wave, AEVec2{ player.x, player.y }, wave_meshes);

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

    enemy1.UpdateEnemystate(player,deltaTime, enemy_wave,wave_meshes,dir_wave);

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

    dead = PlayerEnemyCollision(player, { enemy1.x, enemy1.y }, tilesize);

    // Inside your game loop:
    if (dead && !gameOverSoundPlayed)
    {
        AudioManager::PlayAudio("GameOver", 1.0f, 1.0f, 0);
        gameOverSoundPlayed = true;  // Ensure it doesn't play again
    }

    winGame = checkGoal(player, maze, tilesize, 0.0f, 0.0f);
    if (winGame && !gameWinSoundPlayed)
    {
        AudioManager::PlayAudio("Win", 1.0f, 1.0f, 0);
        gameWinSoundPlayed = true;  // Ensure it doesn't play again
    }
    
   // std::cout << "playerpos " << player.x << "," << player.y << "\n";

}

void GameState1::Draw() {
    AEGfxSetCamPosition(player.x, player.y);
	if (!PauseOverlay::IsPaused()) {
		rock::drawRock(player, maze, tilesize, 0.0f, 0.0f, wave, wave_meshes, rockMesh, rockTexture);
	}
    RenderMesh(enemyMesh, enemy1.x, enemy1.y, enemy1.w, enemy1.h, 0.0f);

    /*RenderMesh(PlayerMesh, player.x, player.y, 1.0f, 1.0f, 45.0f);*/
    //(enemyMesh, enemy1.x, enemy1.y, enemy1.w, enemy1.h, 0.0f);

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

    RenderMaze(maze, tilesize, wallMesh, pathMesh, goalMesh, keyMesh, rockMesh, keyTexture, rockTexture);
    DrawInventory(player, boxMesh, keyMesh, rockMesh, keyTexture, rockTexture);

    AEMtx33 identityMtx;
    AEMtx33Identity(&identityMtx);
    AEGfxSetTransform(identityMtx.m);
    AEGfxTextureSet(nullptr, 0, 0);
        // Explicitly reset the texture after the loop finishes
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
    AEGfxTextureUnload(keyTexture);
    AEGfxTextureUnload(rockTexture);
    worldItems.clear();
    
    for (int i{};i < INVENTORY_SIZE;++i) {
        player.inventory[i] = NOTHING;
    }
    PauseOverlay::Free();
}

void GameState1::Unload() {
    AudioManager::UnloadAudio("GameBGM");
	AudioManager::UnloadAudio("Waves");
	AudioManager::UnloadAudio("Rock Throw");
	AudioManager::UnloadAudio("Collide");
	//AudioManager::UnloadAudio("GameOver");
	//AudioManager::UnloadAudio("Win");
	AudioManager::UnloadAudio("Collect");
}

BaseState* GameState1::GetNextState() {
    if (exitGame) {
        return new MainMenuState();
    }
    if (dead) {
        return new LoseState();
    }
    if (winGame) {
        return new gameTransition(new gamePreview(2));
    }
    return nullptr;  // Stay in the current state if nothing changes
}
