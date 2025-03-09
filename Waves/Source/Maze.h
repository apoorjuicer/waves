/* Start Header ************************************************************************/
/*!
\file		Maze.h
\author		Jordain
\co-author	Guan Shao Jun
\email		jordainyingzhe.ng@digipen.edu

\brief

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include "AEEngine.h"
#include <iostream>


constexpr int ROW = 10;
constexpr int COL = 10;
constexpr float TILE_SIZE = 100.f;

extern int maze[10][10];

//int maze[ROW][COL] = {
//    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
//    { 1, 0, 0, 0, 0, 0, 1, 0, 2, 1 },
//    { 1, 0, 1, 1, 1, 0, 1, 0, 1, 1 },
//    { 1, 3, 4, 0, 1, 0, 0, 0, 0, 1 },
//    { 1, 1, 1, 0, 1, 1, 1, 1, 0, 1 },
//    { 1, 0, 0, 0, 0, 0, 0, 1, 0, 1 },
//    { 1, 0, 1, 1, 1, 1, 0, 1, 0, 1 },
//    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
//    { 1, 1, 1, 1, 1, 1, 1, 1, 0, 1 },
//    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
//};

void RenderMaze(int maze1[ROW][COL], float tileSize, AEGfxVertexList* wallMesh, AEGfxVertexList* pathMesh, AEGfxVertexList* goalMesh, AEGfxVertexList* keyMesh, AEGfxVertexList* rockMesh);
AEGfxVertexList* CreateSquareMesh(float size, u32 color);
