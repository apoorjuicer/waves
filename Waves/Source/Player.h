/* Start Header ************************************************************************/
/*!
\file		mainMenu.h
\author		Jordain Ng
\co-author  Guan Shao Jun
\email		jordainyingzhe.ng@digipen.edu

\brief      this file includes functions, to for player movement and collision checking, to
            prevent the player from going into the wall

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include<iostream>
#include <vector>
#include "Maze.h"
//#include "Enemy.h"

enum InventoryItemType {
    NOTHING = 0,
    KEY,
    ROCK
};

struct WorldItem {
    InventoryItemType type;
    f32 x, y;
    bool collected;

    WorldItem(InventoryItemType t, f32 posX, f32 posY) : type(t), x(posX), y(posY), collected(false) {}
};

const int INVENTORY_SIZE = 3;

//struct Enemy;
//class Enemy;


struct Player {
    f32 x, y, rad=10.0f;  // Position in world space
    InventoryItemType inventory[3] = { NOTHING, NOTHING, NOTHING };
    //InventoryItem inventory[INVENTORY_SIZE] = { ROCK, NOTHING, NOTHING };'
    AEVec2 lastRockPos{0.f,0.f};
    bool rockActive = false;
    bool rockLanded = false;
    void PickUpItem(InventoryItemType itemType);
    void DropItem(f32 dropX, f32 dropY);
};

extern std::vector<WorldItem> worldItems;
extern Player player; // Start at (50, 50) in world space
//extern Player lastRock;
AEGfxVertexList* CreateDonutMesh(f32 outerRadius, f32 innerRadius, int numSegments, u32 color);
void RenderMesh(AEGfxVertexList* NameOfMesh, f32 posX, f32 posY, f32 scaleX, f32 scaleY, f32 rotation);
AEGfxVertexList* Player_Mesh(int NumOfSlices, u32 colour, f32 radius);
bool IsWalkable(f32 x, f32 y, int maze[MAX_ROW][MAX_COL], f32 tileSize, f32 mazeStartX, f32 mazeStartY);
void MovePlayer(Player& pplayer, f32 dx, f32 dy, f32 speed, f32 dt, int maze[MAX_ROW][MAX_COL], f32 tileSize);
void RenderPlayer(const Player& player, AEGfxVertexList* playerMesh, AEGfxTexture* playerTexture);
void CheckItem(Player& player, int maze[MAX_ROW][MAX_COL], f32 tileSize, f32 mazeStartX, f32 mazeStartY);
bool PlayerEnemyCollision(Player &player, AEVec2 enemy_pos, f32 tilesize);
bool checkGoal(Player& player, int maze[MAX_ROW][MAX_COL], f32 tileSize, f32 mazeStartX, f32 mazeStartY);
AEGfxVertexList* CreateBoxMesh(u32 color);
void DrawInventory(const Player& player1, AEGfxVertexList* boxMesh,AEGfxVertexList* keyMesh, AEGfxVertexList* rockMesh,AEGfxTexture* keyTexture, AEGfxTexture* rockTexture);
//bool PlayerEnemyCollision(Player &player, Enemy &enemy);
