#pragma once
#include<iostream>
#include "Maze.h"
#include "Enemy.h"

struct Enemy;

struct Player {
    f32 x, y, rad=10.0f;  // Position in world space
    s8 key=0;
};

extern Player player; // Start at (50, 50) in world space
extern Player lastRock;
AEGfxVertexList* CreateDonutMesh(f32 outerRadius, f32 innerRadius, int numSegments, u32 color);
void RenderMesh(AEGfxVertexList* NameOfMesh, f32 posX, f32 posY, f32 scaleX, f32 scaleY, f32 rotation);
AEGfxVertexList* Player_Mesh(int NumOfSlices, u32 colour, f32 radius);
bool IsWalkable(f32 x, f32 y, int maze[ROW][COL], f32 tileSize, f32 mazeStartX, f32 mazeStartY);
void MovePlayer(Player& pplayer, f32 dx, f32 dy, f32 speed, f32 dt, int maze[ROW][COL], f32 tileSize);
void RenderPlayer(const Player& player, AEGfxVertexList* playerMesh);
void CheckKey(Player& player, int maze[10][10], float tileSize, float mazeStartX, float mazeStartY);
bool PlayerEnemyCollision(Player &player, Enemy &enemy);
bool checkGoal(Player& player, int maze[ROW][COL], f32 tileSize, f32 mazeStartX, f32 mazeStartY);
//bool PlayerEnemyCollision(Player &player, Enemy &enemy);
