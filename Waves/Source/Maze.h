/* Start Header ************************************************************************/
/*!
\file		Maze.h
\author		Jordain
\co-author
\email

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

extern int maze[10][10];


extern int maze[ROW][COL];

void RenderMaze(int maze[ROW][COL], float tileSize, AEGfxVertexList* wallMesh, AEGfxVertexList* pathMesh);
AEGfxVertexList* CreateSquareMesh(float size, u32 color);
