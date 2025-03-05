#pragma once

#define TILE_SIZE 100.0f

extern int maze[10][10];

void RenderMaze(int maze[10][10], float tileSize, AEGfxVertexList* wallMesh, AEGfxVertexList* pathMesh);
AEGfxVertexList* CreateSquareMesh(float size, u32 color);
