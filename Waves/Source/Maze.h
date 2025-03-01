#pragma once

extern int maze[20][20];

void RenderMaze(int maze[20][20], float tileSize, AEGfxVertexList* wallMesh, AEGfxVertexList* pathMesh);
AEGfxVertexList* CreateSquareMesh(float size, u32 color);
