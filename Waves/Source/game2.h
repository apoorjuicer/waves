/* Start Header ************************************************************************/
/*!
\file		game2.h
\project    Waves

\author		Henry Hu
\co-author  Jordain Ng(15%), Benjamin Ban(15%), Chewn Thing Kwan(15%), Guan Shao Jun(15%)
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

#pragma once
#include "pch.h"

#include "Player.h"
#include "Maze.h"
#include "Rock.h"
#include "Wave.h"
#include "Enemy.h"
#include "mainMenu.h"
#include "winState.h"
#include "loseState.h"
#include "pauseOverlay.h"
#include "Audio.h"

// ========================================================
// GameState2 class for managing the core gameplay
// ========================================================
class GameState2 : public BaseState {
private:
    // ====================================================
    // Boolean variables for game state management
    // ====================================================
    bool exitGame = false;
    bool dead = false;
    bool winGame = false;
    bool gameOverSoundPlayed = false;  
    bool gameWinSoundPlayed = false;  

    f32 tilesize = 100.0f; // Tile size for the game grid

    // ====================================================
    // Meshes and textures for rendering
    // ====================================================
    AEGfxVertexList* pDonutMesh = nullptr;
    AEGfxVertexList* PlayerMesh = nullptr;
    AEGfxVertexList* wallMesh = nullptr;
    AEGfxVertexList* pathMesh = nullptr;
    AEGfxVertexList* enemyMesh = nullptr;
    AEGfxVertexList* keyMesh = nullptr;
    AEGfxVertexList* rockMesh = nullptr;
    AEGfxVertexList* goalMesh = nullptr;
    AEGfxVertexList* boxMesh = nullptr;
    AEGfxVertexList* wave_meshes[NUM_WAVE_MESHES] = {};
    AEGfxTexture* keyTexture;
    AEGfxTexture* rockTexture;
    AEGfxTexture* playerTexture;
    AEGfxTexture* pathTexture;
    AEGfxTexture* doorTexture;
    AEGfxTexture* enemyTexture;

    // ====================================================
    // Game objects and variables
    // ====================================================
    int prevDestX = -1;
    int prevDestY = -1;

    // ====================================================
    // Directional Wave and Wave objects
    // ====================================================
    Player player;

    DirectionalWave dir_wave;
    Wave wave[MAX_NUM_WAVES];
    Wave enemy_wave[MAX_NUM_WAVES];
    Wave reflections[MAX_NUM_REFLECTIONS];
    Outline outlines[MAX_NUM_OUTLINES];

public:
    GameState2();                           // Constructor
    void Load() override;                   // Load assets
    void Initialize() override;             // Initialize data
    void Update(f32 deltaTime) override;    // Handle logic updates
    void Draw() override;                   // Render game objects
    void Free() override;                   // Free runtime resources
    void Unload() override;                 // Unload assets
    BaseState* GetNextState() override;     // Return the next state
};

