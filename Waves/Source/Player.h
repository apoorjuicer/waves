#pragma once
#include<iostream>
#include "Maze.h"
AEGfxVertexList* CreateDonutMesh(f32 outerRadius, f32 innerRadius, int numSegments, u32 color);
void RenderMesh(AEGfxVertexList* NameOfMesh, f32 posX, f32 posY, f32 scaleX, f32 scaleY, f32 rotation);
AEGfxVertexList* Player_Mesh(int NumOfSlices, u32 colour, f32 radius);

//struct playermovement
//{
//	f32 x;
//	f32 y;
//};
//
//extern struct playermovement playermove;
//
//bool IsWalkable(float x, float y, int maze[10][10], float tileSize, int number_of_tiles); //number of tiles means the number of tiles height and width
//
//void PlayerMovement(playermovement& playmove, f32 x, f32 y, f32 dt, int maze[10][10], f32 tilesize);
struct Player {
    f32 x, y;  // Position in world space
};

extern Player player; // Start at (50, 50) in world space
extern Player lastRock;

bool IsWalkable(f32 x, f32 y, int maze[ROW][COL], f32 tileSize, f32 mazeStartX, f32 mazeStartY);
void MovePlayer(Player& player, f32 dx, f32 dy, f32 speed, f32 dt, int maze[10][10], f32 tileSize);
void RenderPlayer(const Player& player, AEGfxVertexList* playerMesh);
