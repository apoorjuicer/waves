#pragma once
#define maze_size 20
extern int maze[maze_size][maze_size];


void RenderMaze(int maze[maze_size][maze_size], float tileSize, AEGfxVertexList* wallMesh, AEGfxVertexList* pathMesh);
AEGfxVertexList* CreateSquareMesh(float size, u32 color);
