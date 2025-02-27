#pragma once
#include "baseStates.h"
#include "AEEngine.h"
#include "Player.h"
#include "Maze.h"
#include "Rock.h"
#include "enemy_henrey.h"
#include "sonar.hpp"

class GameState : public BaseState {
private:
    bool exitGame = false;
    f32 tilesize = 100.0f;
    AEGfxVertexList* pDonutMesh = nullptr;
    AEGfxVertexList* PlayerMesh = nullptr;
    AEGfxVertexList* wallMesh = nullptr;
    AEGfxVertexList* pathMesh = nullptr;
    AEGfxVertexList* enemy1 = nullptr;
    AEGfxVertexList* wave_meshes[NUM_WAVE_MESHES] = {};
    wave_s wave[MAX_NUM_WAVES] = {};
    wave_s reflection_waves[MAX_NUM_WAVES] = {};
    line_s outlines[MAX_NUM_OUTLINES] = {};
    wave_c class_waves_array[MAX_NUM_WAVES]{};

public:
    GameState();
    void Load() override;
    void Initialize() override;
    void Update(float deltaTime) override;
    void Draw() override;
    void Free() override;
    void Unload() override;
    BaseState* GetNextState() override;
};

