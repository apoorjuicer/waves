#pragma once
#include<iostream>

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
    float x, y;  // Position in world space
    int key=0;
    int lives = 0;
};

extern Player player; // Start at (50, 50) in world space

bool IsWalkable(float x, float y, int maze[10][10], float tileSize, float mazeStartX, float mazeStartY);
void CheckKey(Player& player, int maze[10][10], float tileSize, float mazeStartX, float mazeStartY);
void MovePlayer(Player& player, float dx, float dy, float speed, float dt, int maze[10][10], float tileSize);
void RenderPlayer(const Player& player, AEGfxVertexList* playerMesh);
void DrawLives(int Lives, AEGfxVertexList* heartMesh, AEGfxTexture* heartTexture);
