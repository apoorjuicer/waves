#pragma once
#include "baseStates.h"
#include "AEEngine.h"
#include "Player.h"
#include "Maze.h"
#include "Rock.h"
//#include "enemy_henrey.h"
#include "Wave.hpp"
#include "Enemy.h"

class GameState : public BaseState {
private:
    bool exitGame = false;
    bool winGame = false;
    f32 tilesize = 100.0f;
    AEGfxVertexList* pDonutMesh = nullptr;
    AEGfxVertexList* PlayerMesh = nullptr;
    AEGfxVertexList* wallMesh = nullptr;
    AEGfxVertexList* pathMesh = nullptr;
    AEGfxVertexList* enemyMesh = nullptr;
    AEGfxVertexList* wave_meshes[NUM_WAVE_MESHES] = {};
    int prevDestX = -1;
    int prevDestY = -1;
    //wave_s wave[MAX_NUM_WAVES] = {};
    //wave_s reflection_waves[MAX_NUM_WAVES] = {};
    //line_s outlines[MAX_NUM_OUTLINES] = {};

    Wave wave[MAX_NUM_WAVES];
    Wave reflections[MAX_NUM_REFLECTIONS];
    Outline outlines[MAX_NUM_OUTLINES];

public:
    GameState();
    void Load() override;
    void Initialize() override;
    void Update(f32 deltaTime) override;
    void Draw() override;
    void Free() override;
    void Unload() override;
    BaseState* GetNextState() override;
};

