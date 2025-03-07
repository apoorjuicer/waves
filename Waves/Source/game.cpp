#include "game.h"
#include "mainMenu.h"


GameState::GameState() : exitGame(false), tilesize(100.0f),
pDonutMesh(nullptr), PlayerMesh(nullptr), wallMesh(nullptr),
goalMesh(nullptr), pathMesh(nullptr), enemy1(nullptr) {

}

void GameState::Load() {
    AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

    pDonutMesh = CreateDonutMesh(1000.0f, 100.0f, 48, 0x00000000);
    PlayerMesh = Player_Mesh(48, 0xFFFFFFFF, 10.0f);
    wallMesh = CreateSquareMesh(tilesize, 0xFF000000);
    pathMesh = CreateSquareMesh(tilesize, 0x55555555);
    goalMesh = CreateSquareMesh(tilesize, 0x5500FF00);
    heartMesh = CreateSquareMesh(0.5f, 0x5500FF00);
    heartTexture = AEGfxTextureLoad("..\\Assets\\heart.png");
    //heartTexture = AEGfxTextureLoad("C:\\Users\\shaoj\\Documents\\GitHub\\csd1451s25_team_taurine\\Waves\\Assets\\heart.png");
    if (!heartTexture) {
        std::cerr << "Error: Failed to load heart texture!" << std::endl;
        exit(1);
    }
    enemy1 = createEnemyMesh(0xFFFF0000);
    RockMesh = rock::Rock_Mesh(48, 0xFFFFFFFF, 10.0f);

    wave_meshes[0] = CreateWaveMesh(1.0f, 0.95f, 36);
    wave_meshes[1] = Create_SemiCircle_Mesh(1.0f, 0.98f, 36, 2);
    wave_meshes[2] = Create_LineMesh();

}

void GameState::Initialize() {
    exitGame = false;
}

void GameState::Update(float deltaTime) {
    // Exit GameState when ESC is pressed
    if (AEInputCheckTriggered(AEVK_ESCAPE)) {
        exitGame = true;
    }
    AEGfxSetBackgroundColor(0.1f, 0.1f, 0.1f);

    // Player movement
    float speed = 360.0f;
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

    CheckKey(player, maze, tilesize, 0.0f, 0);
}

void GameState::Draw() {
    rock::drawRock(player, maze, tilesize, 0.0f, 0.0f, wave, wave_meshes, RockMesh);

    RenderMesh(PlayerMesh, player.x, player.y, 1.0f, 1.0f, 45.0f);
    RenderMesh(enemy1, 350.0f, -100.0f, 50.0f, 50.0f, 0.0f);

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

    RenderMaze(maze, tilesize, wallMesh, pathMesh, goalMesh,RockMesh);
    //RenderMesh(pDonutMesh, player.x, player.y, 1.0f, 1.0f, 45.0f);
    RenderPlayer(player, PlayerMesh);
    //DrawLives(player.lives, heartMesh, heartTexture);
    AEGfxSetCamPosition(player.x, player.y);
}

void GameState::Free() {
    for (AEGfxVertexList* (&i) : wave_meshes) {
        if (i) {
            AEGfxMeshFree(i);
        }
    }

    //if (heartTexture) {
    //    AEGfxTextureUnload(heartTexture);
    //    heartTexture = nullptr;
    //}

    //if (heartMesh) {
    //    AEGfxMeshFree(heartMesh);
    //    heartMesh = nullptr;
    //}
}

void GameState::Unload() {}

BaseState* GameState::GetNextState() {
    return exitGame ? new MainMenuState() : nullptr;
}
