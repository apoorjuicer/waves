#include "game.h"
#include "mainMenu.h"
#include "vector"
#include "enemy_henrey.h"


GameState::GameState() : exitGame(false), tilesize(100.0f),
pDonutMesh(nullptr), PlayerMesh(nullptr), wallMesh(nullptr),
pathMesh(nullptr), enemy1(nullptr) {

}

void GameState::Load() {
    AEGfxSetBackgroundColor(0.5f, 0.5f, 0.5f);

    pDonutMesh = CreateDonutMesh(1000.0f, 100.0f, 48, 0x00000000);
    PlayerMesh = Player_Mesh(48, 0xFFFFFFFF, 10.0f);
    wallMesh = CreateSquareMesh(tilesize, 0xFF000000);
    pathMesh = CreateSquareMesh(tilesize, 0x55555555);
    enemy1 = createEnemyMesh(0xFFFF0000);

    wave_meshes[0] = CreateWaveMesh(1.0f, 0.95f, 36);
    wave_meshes[1] = Create_SemiCircle_Mesh(1.0f, 0.98f, 36, 2);
    wave_meshes[2] = Create_LineMesh(false);
    wave_meshes[3] = Create_LineMesh(true);

    init_reflection_waves(reflection_waves, wave_meshes[1]);
}

void GameState::Initialize() {
    exitGame = false;
}

void GameState::Update(float deltaTime) {
    // Exit GameState when ESC is pressed
    if (AEInputCheckTriggered(AEVK_ESCAPE)) {
        exitGame = true;
    }

    // Update wave behavior
    for (wave_s& i : wave) {
        if (i.alive) {
            update_waves(i);
            reflections_spawner(i, maze, tilesize, reflection_waves, outlines, wave_meshes);
        }
    }

    for (wave_s& i : reflection_waves) {
        if (i.alive) {
            update_waves(i);
        }
    }

    for (line_s& l : outlines) {
        if (l.alive) {
            update_outlines(l);
        }
    }

    // Player movement
    float speed = 120.0f;
    if (AEInputCheckCurr(AEVK_W)) MovePlayer(player, 0.0f, 1.0f, speed, deltaTime, maze, tilesize);
    if (AEInputCheckCurr(AEVK_S)) MovePlayer(player, 0.0f, -1.0f, speed, deltaTime, maze, tilesize);
    if (AEInputCheckCurr(AEVK_A)) MovePlayer(player, -1.0f, 0.0f, speed, deltaTime, maze, tilesize);
    if (AEInputCheckCurr(AEVK_D)) MovePlayer(player, 1.0f, 0.0f, speed, deltaTime, maze, tilesize);

    if (AEInputCheckTriggered(AEVK_SPACE)) {
        spawn_wave(wave, player.x, player.y, wave_meshes[0]);
    }
}

void GameState::Draw() {
    RenderPlayer(player, PlayerMesh);
    rock::drawRock(player, maze, tilesize, 0.0f, 0.0f, wave, wave_meshes[0]);

    RenderMesh(PlayerMesh, player.x, player.y, 1.0f, 1.0f, 45.0f);
    //RenderMesh(enemy1, 350.0f, -100.0f, 50.0f, 50.0f, 0.0f);
    //RenderMesh(pDonutMesh, player.x, player.y, 1.0f, 1.0f, 45.0f);


    std::vector<std::pair<int, int>> enemyPath;  // Path for enemy movement
    // Enemy's current position
    float enemyX = 350.0f, enemyY = -100.0f;

    // Target Position (Goal)
    int goalX = 8, goalY = 8; // Example: Goal in the grid

    // Convert enemy world position to grid indices
    int enemyGridX = (int)(enemyX / tilesize);
    int enemyGridY = (int)(enemyY / tilesize);

    // Compute path only once at the start
    enemyPath = AStarPathfinding(enemyGridX, enemyGridY, goalX, goalY);

    if (!enemyPath.empty()) {
        // Move to the next step in the path
        enemyX = enemyPath[0].first * tilesize;
        enemyY = enemyPath[0].second * tilesize;

        // Remove the first step (since we moved)
        enemyPath.erase(enemyPath.begin());
    }

    // Render the enemy at its updated position
    RenderMesh(enemy1, enemyX, enemyY, 50.0f, 50.0f, 0.0f);


    draw_wave(wave);
    draw_wave(reflection_waves);

    for (line_s& l : outlines) {
        if (l.alive) {
            draw_outlines(l, wave_meshes);
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
}

void GameState::Unload() {}

BaseState* GameState::GetNextState() {
    return exitGame ? new MainMenuState() : nullptr;
}
