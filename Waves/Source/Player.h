#pragma once
#include<iostream>
#include "Maze.h"

struct Player {
    f32 x, y;  // Position in world space
    int key=0;
    int lives = 3;
};

extern Player player; // Start at (50, 50) in world space
extern Player lastRock;
AEGfxVertexList* CreateDonutMesh(f32 outerRadius, f32 innerRadius, int numSegments, u32 color);
void RenderMesh(AEGfxVertexList* NameOfMesh, f32 posX, f32 posY, f32 scaleX, f32 scaleY, f32 rotation);
AEGfxVertexList* Player_Mesh(int NumOfSlices, u32 colour, f32 radius);
bool IsWalkable(f32 x, f32 y, int maze[ROW][COL], f32 tileSize, f32 mazeStartX, f32 mazeStartY);
void MovePlayer(Player& pplayer, f32 dx, f32 dy, f32 speed, f32 dt, int maze[ROW][COL], f32 tileSize);
void CheckKey(Player& player, int maze[10][10], float tileSize, float mazeStartX, float mazeStartY);
void RenderPlayer(const Player& player, AEGfxVertexList* playerMesh);
void DrawLives(int Lives, AEGfxVertexList* heartMesh, AEGfxTexture* heartTexture);
