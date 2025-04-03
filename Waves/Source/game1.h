/* Start Header ************************************************************************/
/*!
\file		game1.h
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

#pragma once
#include "baseStates.h"
#include "AEEngine.h"
#include "gameTransition.h"
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

class GameState1 : public BaseState {
private:
    bool exitGame = false;
    bool dead = false;
    bool winGame = false;
    f32 tilesize = 100.0f;
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
    int prevDestX = -1;
    int prevDestY = -1;
    bool gameOverSoundPlayed = false;  // Declare this outside the game loop
    bool gameWinSoundPlayed = false;  // Declare this outside the game loop


    DirectionalWave dir_wave;
    Wave wave[MAX_NUM_WAVES];
    Wave enemy_wave[MAX_NUM_WAVES];
    Wave reflections[MAX_NUM_REFLECTIONS];
    Outline outlines[MAX_NUM_OUTLINES];

public:
    GameState1();
    void Load() override;
    void Initialize() override;
    void Update(f32 deltaTime) override;
    void Draw() override;
    void Free() override;
    void Unload() override;
    BaseState* GetNextState() override;
};

