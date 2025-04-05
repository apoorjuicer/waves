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

extern AEVec2 GlobalCenter;

struct Player {
    enum PLAYER_STATE { IS_MOVING, IS_IDLE, FACING_LEFT, FACING_RIGHT };

    f32 x, y, rad=10.0f;  // Position in world space
    
    Player() : x{}, y{}, texture{ nullptr }, mesh{ nullptr } {}

    PLAYER_STATE state = IS_IDLE;
    PLAYER_STATE dir = FACING_RIGHT;

    AEGfxTexture* texture;
    AEGfxVertexList* mesh;

    InventoryItemType inventory[3] = { NOTHING, NOTHING, NOTHING };

    AEVec2 lastRockPos{0.f,0.f};
    bool rockActive = false;
    bool rockLanded = false;
    void PickUpItem(InventoryItemType itemType);
    void DropItem(f32 dropX, f32 dropY);
};

extern std::vector<WorldItem> worldItems;

AEGfxVertexList* CreateDonutMesh(f32 outerRadius, f32 innerRadius, int numSegments, u32 color);
void RenderMesh(AEGfxVertexList* NameOfMesh, f32 posX, f32 posY, f32 scaleX, f32 scaleY, f32 rotation);

bool IsWalkable(f32 x, f32 y, int maze[MAX_ROW][MAX_COL], f32 tileSize, f32 mazeStartX, f32 mazeStartY);
void MovePlayer(Player& pplayer, f32 dx, f32 dy, f32 speed, f32 dt, int maze[MAX_ROW][MAX_COL], f32 tileSize);

void DrawPlayer(Player const& player);
void DrawTexturedEntity(AEVec2 pos, AEGfxTexture* texture, AEGfxVertexList* mesh, AEVec2 offset = {0.f,0.f});

void CheckItem(Player& player);
bool PlayerEnemyCollision(Player &player, AEVec2 enemy_pos, f32 tilesize);
bool checkGoal(Player& player, int maze[MAX_ROW][MAX_COL], f32 tileSize, f32 mazeStartX, f32 mazeStartY);
AEGfxVertexList* CreateBoxMesh(u32 color);
void DrawInventory(const Player& player1, AEGfxVertexList* boxMesh,AEGfxVertexList* keyMesh, AEGfxVertexList* rockMesh,AEGfxTexture* keyTexture, AEGfxTexture* rockTexture);
//bool PlayerEnemyCollision(Player &player, Enemy &enemy);

