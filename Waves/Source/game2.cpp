/* Start Header ************************************************************************/
/*!
\file		game2.cpp
\project    Waves

\author		Henrey Hu
\email		zhihenghenrey.hu@digipen.edu
\co-author  Jordain Ng(15%), Benjamin Ban(15%), Chewn Thing Kwan(15%), Guan Shao Jun(15%)

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

// Enemy object
namespace {
    Enemy enemy2;
}

// Constructor
GameState2::GameState2() : exitGame(false), winGame(false), tilesize(100.0f),
pDonutMesh(nullptr), PlayerMesh(nullptr), wallMesh(nullptr),
pathMesh(nullptr), enemyMesh(nullptr), rockMesh(nullptr), keyMesh(nullptr),
goalMesh(nullptr), boxMesh(nullptr), keyTexture(nullptr), rockTexture(nullptr),
playerTexture(nullptr), pathTexture(nullptr), doorTexture(nullptr), enemyTexture(nullptr),
prevDestX(-1), prevDestY(-1), gameOverSoundPlayed(false),
gameWinSoundPlayed(false) {
}

// Load assets
void GameState2::Load() {
    // Ensure old maze is wiped
    ClearMazeData(); 

    // Ensure old world items are wiped
    worldItems.clear();

    // Read maze data from file
    if (!ImportMapDataFromFile("Assets/Maze2.txt")) {
        std::cerr << "Error: Failed to load maze file Assets/Maze1.txt. Quitting game.\n";
    }

    // ==========================================================
    // Load textures and meshes
    // ==========================================================
    player.texture = AEGfxTextureLoad("Assets/Textures/Texture_Player.png");
    player.mesh = CreateTextureMesh(4, 2);

    pDonutMesh = CreateDonutMesh(1000.0f, 120.0f, 48, 0xFF000000);
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

    // Create world items
    createWorldItems(tilesize);

    // Set enemy patrol path
    enemy2.setEnemyPatrol(maze);
}

// Initialize data
void GameState2::Initialize() {
    // ============================================
    // Initialize bool variables
    // ============================================
    exitGame = false;
    dead = false;
    winGame = false;

    // ============================================
    // Initialize player and inventory
    // ============================================
    player.x = 100.f;
    player.y = -100.f;
    player.inventory[0] = ROCK;
    player.lastRockPos = {};

    // ===========================================
    // Set up enemy patrol
    // ===========================================
    enemy2.startPatrol = enemy2.patrolPath.front();
    enemy2.endPatrol = enemy2.patrolPath.back();
    enemy2.x = enemy2.startPatrol.first * TILE_SIZE;
    enemy2.y = enemy2.startPatrol.second * -TILE_SIZE;
    enemy2.state = IDLE;
    enemy2.previousState = IDLE;
    //enemy2.idle_timer = 3.f;
    enemy2.patrol_timer = 20.f;
    //enemy2.pursuit_timer = 3.f;
    enemy2.updateGridPosition(); // need to call to init conversion
    
    // ===========================================
    // Load and play audio
    // ===========================================
    AEAudioGroup InGame = AEAudioCreateGroup();  // For background music
    AEAudioGroup RockThrow = AEAudioCreateGroup();  // Diff Grp to cancel sound
    AudioManager::LoadAudio("GameBGM", "Assets/Audio/GameBGM.mp3", true, InGame);
    AudioManager::LoadAudio("Waves", "Assets/Audio/SonarWave.wav", false, InGame);
    AudioManager::LoadAudio("Rock Throw", "Assets/Audio/Throw.wav", false, RockThrow);
    AudioManager::LoadAudio("Collide", "Assets/Audio/Collision.wav", false, InGame);
    AudioManager::LoadAudio("Collect", "Assets/Audio/Collect.wav", false, InGame);
    AudioManager::PlayAudio("GameBGM", 0.3f, 1.0f, -1);

    // Initialize pause overlay
    PauseOverlay::Init();
}

// Handle logic updates
void GameState2::Update(f32 deltaTime) {
    if (!AudioManager::IsAudioPlaying("GameBGM")) {
        AudioManager::PlayAudio("GameBGM", 0.3f, 1.0f, -1);
    }

    // ===========================================
    // Pause Overlay
    // ===========================================
    if (AEInputCheckTriggered(AEVK_ESCAPE) && !PauseOverlay::IsPaused()) {
        PauseOverlay::SetPaused(true);
        AEInputReset();
    }

    if (PauseOverlay::IsPaused()) {
        PauseOverlay::Update();
        if (PauseOverlay::ShouldExitToMenu()) {
            exitGame = true;
        }
        return;
    }

    // ==========================================
    // Player movement
    // ==========================================
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

    // Check for directional waves
    if (AEInputCheckTriggered(AEVK_SPACE) && !dir_wave.is_active) {
        // spawn a directional wave towards player mouse;
        spawn_dir_wave(dir_wave, AEVec2{ player.x, player.y }, wave_meshes);
        AudioManager::PlayAudio("Waves", 1.0f, 1.0f, 0); // Play wave sound once
    }

    enemy2.UpdateEnemystate(player, deltaTime, enemy_wave, wave_meshes, dir_wave);

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
    dead = PlayerEnemyCollision(player, {enemy2.x, enemy2.y}, tilesize);
    winGame = checkGoal(player, maze, tilesize, 0.0f, 0.0f);
    if (dead && !gameOverSoundPlayed)
    {
        AudioManager::PlayAudio("GameOver", 1.0f, 1.0f, 0);
        gameOverSoundPlayed = true; 
    }

    if (winGame && !gameWinSoundPlayed)
    {
        AudioManager::PlayAudio("Win", 1.0f, 1.0f, 0);
        gameWinSoundPlayed = true;  
    }
}

// Render game objects
void GameState2::Draw() {
    // Center camera on player
    AEGfxSetCamPosition(GlobalCenter.x, GlobalCenter.y);

    // ============================================
    // Draw maze, player, enemies, and other game objects
    // ============================================
    RenderBackground(tilesize, pathMesh, pathTexture);
    RenderItems(keyMesh, rockMesh, keyTexture, rockTexture);
    DrawTexturedEntity(AEVec2{ enemy2.x, enemy2.y }, enemyTexture, enemyMesh);

    // Draw the rock if the player is not paused
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

// Free runtime resources
void GameState2::Free() {
	// ============================================
	// Free all meshes and textures
	// ============================================
    for (AEGfxVertexList* (&i) : wave_meshes) {
        if (i) {
            AEGfxMeshFree(i);
        }
    }

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

    // Free all world items
    worldItems.clear();
    for (int i{};i < INVENTORY_SIZE;++i) {
        player.inventory[i] = NOTHING;
    }

    // Free pause overlay resources
    PauseOverlay::Free();
}

// Unload assets
void GameState2::Unload() {
    AudioManager::UnloadAudio("GameBGM");
    AudioManager::UnloadAudio("Waves");
    AudioManager::UnloadAudio("Rock Throw");
    AudioManager::UnloadAudio("Collide");
	AudioManager::UnloadAudio("Collect");
}

// Return the next state
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
