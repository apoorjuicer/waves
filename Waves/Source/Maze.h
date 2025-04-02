/* Start Header ************************************************************************/
/*!
\file		Maze.h
\author		Jordain
\co-author	Guan Shao Jun
\email		jordainyingzhe.ng@digipen.edu

\brief      This folder includes functions, to help with drawing of the maze, with a 2d array

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include "AEEngine.h"
#include "GSM.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


constexpr int MAX_ROW = 20;
constexpr int MAX_COL = 20;

// Global variables for the current maze dimensions:
extern int activeRow;  // e.g., initially 10 or read from file
extern int activeCol;  // e.g., initially 10 or read from file

// The maze grid: only the region [0..activeRow-1][0..activeCol-1] is used.
extern int maze[MAX_ROW][MAX_COL];

//constexpr int ROW = 10;
//constexpr int COL = 10;
constexpr float TILE_SIZE = 100.0f;

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

// File I/O data reading:
bool ImportMapDataFromFile(const std::string& filename);
// File I/O clearing data
void ClearMazeData();
// void Import_MapData(void);
void createWorldItems(f32 tileSize);
// print maze to std::cout;
void PrintMapData();

void RenderMaze(int maze1[MAX_ROW][MAX_COL], f32 tileSize, AEGfxVertexList* wallMesh, AEGfxVertexList* pathMesh, AEGfxVertexList* goalMesh, AEGfxVertexList* keyMesh, AEGfxVertexList* rockMesh);
AEGfxVertexList* CreateSquareMesh(float size, u32 color);
