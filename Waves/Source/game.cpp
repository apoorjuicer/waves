/* Start Header ************************************************************************/
/*!
\file		game.cpp
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


#include "game.h"
#include "mainMenu.h"
#include "winState.h"
#include "loseState.h"

GameState::GameState() : exitGame(false), winGame(false), tilesize(100.0f),
pDonutMesh(nullptr), PlayerMesh(nullptr), wallMesh(nullptr),
pathMesh(nullptr), enemyMesh(nullptr), rockMesh(nullptr), keyMesh(nullptr), goalMesh(nullptr), boxMesh(nullptr), Font(-1), prevDestX(-1), prevDestY(-1) {

}

namespace {
    Enemy enemy1;
}


void GameState::Load() {
    AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

    Font = AEGfxCreateFont("Assets\\liberation-mono.ttf", 50);
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

}

void GameState::Initialize() {
    exitGame = false;
    dead = false;
    winGame = false;
    player.x = 100.f;
    player.y = -700.f;
    //

    //enemy1.x = 100.f;
    //enemy1.y = -100.f;
 /*   enemy1.x = 100.f;
    enemy1.y = -100.f;*/
    enemy1.startPatrol = { 1,7 };
    enemy1.endPatrol = { 8,7 };
    enemy1.state = PATROL;
    enemy1.patrolPath = { {1,7},{2,7},{3,7},{4,7},{5,7},{6,7},{7,7},{8,7} };
}

void GameState::Update(f32 deltaTime) {

    //===========================================================
    // Use bool var winGame when player won to trigger go winState in GetNextState()
    // After keycard interacted with 'exit' then winGame = 1;
    //=========================================================== 
    
    // Exit GameState when ESC is pressed
    if (AEInputCheckTriggered(AEVK_ESCAPE)) {
        exitGame = true;
    }
    AEGfxSetBackgroundColor(0.1f, 0.1f, 0.1f);

    // Player movement
    f32 speed = 500.0f;
    if (AEInputCheckCurr(AEVK_W)) MovePlayer(player, 0.0f, 1.0f, speed, deltaTime, maze, tilesize);
    if (AEInputCheckCurr(AEVK_S)) MovePlayer(player, 0.0f, -1.0f, speed, deltaTime, maze, tilesize);
    if (AEInputCheckCurr(AEVK_A)) MovePlayer(player, -1.0f, 0.0f, speed, deltaTime, maze, tilesize);
    if (AEInputCheckCurr(AEVK_D)) MovePlayer(player, 1.0f, 0.0f, speed, deltaTime, maze, tilesize);

    if (AEInputCheckTriggered(AEVK_SPACE)) {
        spawn_wave(wave, player.x, player.y, wave_meshes);
    }
    //if (AEInputCheckTriggered(AEVK_E)) {
    //    std::cout << "\n\n\n\n\n";
    //}

    UpdateEnemystate(enemy1, deltaTime);

    for (Wave& w : wave) {
        if (w.is_active) {
            // update wave size;
            w.update();
            w.echolocation(maze, tilesize, reflections, outlines);
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
    dead = PlayerEnemyCollision(player, enemy1);
    winGame = checkGoal(player, maze, tilesize, 0.0f, 0.0f);

}

void GameState::Draw() {
    AEGfxSetCamPosition(player.x, player.y);
    rock::drawRock(player, maze, tilesize, 0.0f, 0.0f, wave, wave_meshes, lastRock);

    RenderMesh(PlayerMesh, player.x, player.y, 1.0f, 1.0f, 45.0f);


    RenderMaze(maze, tilesize, wallMesh, pathMesh, goalMesh, keyMesh, rockMesh);
    RenderMesh(pDonutMesh, player.x, player.y, 1.0f, 1.0f, 45.0f);
    RenderMesh(enemyMesh, enemy1.x, enemy1.y, enemy1.w, enemy1.h, 0.0f);
    for (Wave& w : wave) {
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

    DrawInventory(player, boxMesh, Font, keyMesh, rockMesh);
}

void GameState::Free() {
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
}

void GameState::Unload() {}

BaseState* GameState::GetNextState() {
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
        return new WinState();
    }
    //if (winGame) {
    //    return new WinState();
    //}
    return nullptr;  // Stay in the current state if nothing changes

}
