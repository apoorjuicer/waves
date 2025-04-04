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
#include "pch.h"
#include "Maze.h"
#include "WorldItems.h"

const int INVENTORY_SIZE = 3;

struct Player {
    enum PLAYER_STATE { IS_MOVING, IS_IDLE, FACING_LEFT, FACING_RIGHT };

    f32 x, y, rad = 10.f;
    f32 speed = 200.f;

    PLAYER_STATE state = IS_IDLE;
    PLAYER_STATE dir = FACING_RIGHT;

    AEGfxTexture* texture;
    AEGfxVertexList* mesh;

    InventoryItemType inventory[3] = { ROCK, NOTHING, NOTHING };

    void PickUpItem(InventoryItemType itemType);
    void DropItem(f32 dropX, f32 dropY);

    Player() : x{}, y{}, texture{nullptr}, mesh{nullptr} {}
    //Player(AEGfxTexture* tex, AEGfxVertexList* mesh) : x{ 300.f }, y{ -100.f }, texture{ tex }, mesh{ mesh } {}

    void move();
    void draw(/*AEGfxTexture* pTex, AEGfxVertexList* pMesh*/) const;
};

AEGfxVertexList* CreateTextureMesh(int spritesheet_cols, int spritesheet_rows);

AEGfxVertexList* CreateDonutMesh(f32 outerRadius, f32 innerRadius, int numSegments, u32 color);
void RenderMesh(AEGfxVertexList* NameOfMesh, f32 posX, f32 posY, f32 scaleX, f32 scaleY, f32 rotation);
AEGfxVertexList* Player_Mesh(int NumOfSlices, u32 colour, f32 radius);
bool IsWalkable(f32 x, f32 y, int maze[MAX_ROW][MAX_COL], f32 tileSize, f32 mazeStartX, f32 mazeStartY);
void MovePlayer(Player& pplayer, f32 dx, f32 dy, f32 speed, f32 dt, int maze[MAX_ROW][MAX_COL], f32 tileSize);
void RenderPlayer(const Player& player, AEGfxVertexList* playerMesh, AEGfxTexture* playerTexture);
void CheckItem(Player& player);
//bool PlayerCollision(Player &player, Enemy &enemy, f32 tilesize);
bool checkGoal(Player& player, int maze[MAX_ROW][MAX_COL], f32 tileSize, f32 mazeStartX, f32 mazeStartY);
AEGfxVertexList* CreateBoxMesh(u32 color);
void DrawInventory(const Player& player1, AEGfxVertexList* boxMesh,AEGfxVertexList* keyMesh, AEGfxVertexList* rockMesh,AEGfxTexture* keyTexture, AEGfxTexture* rockTexture);
//bool PlayerEnemyCollision(Player &player, Enemy &enemy);
