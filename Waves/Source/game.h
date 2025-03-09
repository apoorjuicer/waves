/* Start Header ************************************************************************/
/*!
\file		game.h
\author		Henry Hu
\co-author  Jordain Ng, Benjamin Ban, Chewn Thing Kwan, Guan Shao Jun
\email		zhihenghenrey.hu@digipen.edu

\brief

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include "baseStates.h"
#include "AEEngine.h"
#include "Player.h"
#include "Maze.h"
#include "Rock.h"
//#include "enemy_henrey.h"
#include "Wave.h"
#include "Enemy.h"

class GameState : public BaseState {
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
    int prevDestX = -1;
    int prevDestY = -1;

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

