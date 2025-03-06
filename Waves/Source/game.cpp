#include "game.h"
#include "mainMenu.h"

GameState::GameState() : exitGame(false), tilesize(100.0f),
pDonutMesh(nullptr), PlayerMesh(nullptr), wallMesh(nullptr),
pathMesh(nullptr), enemyMesh(nullptr) {

}

namespace {
    Enemy enemy1;
}


void GameState::Load() {
    AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

    pDonutMesh = CreateDonutMesh(1000.0f, 100.0f, 48, 0x00000000);
    PlayerMesh = Player_Mesh(48, 0xFFFFFFFF, 10.0f);
    wallMesh = CreateSquareMesh(tilesize, 0xFF000000);
    pathMesh = CreateSquareMesh(tilesize, 0x55555555);
    enemyMesh = createEnemyMesh(0xFFFF0000);

    wave_meshes[0] = CreateWaveMesh(1.0f, 0.95f, 36);
    wave_meshes[1] = Create_SemiCircle_Mesh(1.0f, 0.98f, 36, 2);
    wave_meshes[2] = Create_LineMesh();

}

void GameState::Initialize() {
    exitGame = false;
    enemy1.x = 100.f;
    enemy1.y = -700.f;
 /*   enemy1.x = 100.f;
    enemy1.y = -100.f;*/
    enemy1.startPatrol = { 1,7 };
    enemy1.endPatrol = { 8,7 };
}

void GameState::Update(float deltaTime) {
    // Exit GameState when ESC is pressed
    if (AEInputCheckTriggered(AEVK_ESCAPE)) {
        exitGame = true;
    }
    AEGfxSetBackgroundColor(0.1f, 0.1f, 0.1f);

    // Player movement
    float speed = 120.0f;
    if (AEInputCheckCurr(AEVK_W)) MovePlayer(player, 0.0f, 1.0f, speed, deltaTime, maze, tilesize);
    if (AEInputCheckCurr(AEVK_S)) MovePlayer(player, 0.0f, -1.0f, speed, deltaTime, maze, tilesize);
    if (AEInputCheckCurr(AEVK_A)) MovePlayer(player, -1.0f, 0.0f, speed, deltaTime, maze, tilesize);
    if (AEInputCheckCurr(AEVK_D)) MovePlayer(player, 1.0f, 0.0f, speed, deltaTime, maze, tilesize);

    if (AEInputCheckTriggered(AEVK_SPACE)) {
        spawn_wave(wave, player.x, player.y, wave_meshes);
    }
    if (AEInputCheckTriggered(AEVK_E)) {
        std::cout << "\n\n\n\n\n";
    }

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

    static float accumulatedTime = 0.0f;
    const float updateInterval = 0.1f;  // Move every 0.1 seconds
    // get dt for this frame
    accumulatedTime += deltaTime;


   // Pair src = enemy1.startPatrol, dst = enemy1.endPatrol;
    enemy1.updateGridPosition();
    Pair src = {enemy1.gridRow, enemy1.gridCol }, dst = { 7, 8 };   //row col
    if (!enemy1.reachPath) {  // Only calculate path once
        astarsearch(maze, src, dst, enemy1.path);
        enemy1.reachPath = true;  //  Mark path as calculated
    }

    // Move enemy if enough time has passed
    if (accumulatedTime >= updateInterval) {
        if (enemy1.pathIndex < enemy1.path.size()) {
            enemy1.moveEnemy();
            enemy1.printPosition();
        }
        accumulatedTime = 0.0f;
    }


}

void GameState::Draw() {
    RenderPlayer(player, PlayerMesh);
    rock::drawRock(player, maze, tilesize, 0.0f, 0.0f, wave, wave_meshes);

    RenderMesh(PlayerMesh, player.x, player.y, 1.0f, 1.0f, 45.0f);
    RenderMesh(enemyMesh, enemy1.x, enemy1.y, enemy1.w, enemy1.h, 0.0f);
   // RenderMesh(pDonutMesh, player.x, player.y, 1.0f, 1.0f, 45.0f);

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

    RenderMaze(maze, tilesize, wallMesh, pathMesh);
    AEGfxSetCamPosition(player.x, player.y);
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
}

void GameState::Unload() {}

BaseState* GameState::GetNextState() {
    return exitGame ? new MainMenuState() : nullptr;
}
