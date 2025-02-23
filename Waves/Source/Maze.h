#pragma once

extern int maze[10][10];

void RenderMaze(int maze[10][10], float tileSize, AEGfxVertexList* wallMesh, AEGfxVertexList* pathMesh);
AEGfxVertexList* CreateSquareMesh(float size, u32 color);
