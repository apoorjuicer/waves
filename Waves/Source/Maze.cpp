#include "AEEngine.h"
#include <iostream>
#include "Maze.h"

int maze[ROW][COL] = {
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
    { 1, 0, 0, 0, 0, 0, 1, 0, 2, 1 },
    { 1, 0, 1, 1, 1, 0, 1, 0, 1, 1 },
    { 1, 3, 0, 0, 1, 0, 0, 0, 0, 1 },
    { 1, 1, 1, 0, 1, 1, 1, 1, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 1, 0, 1 },
    { 1, 0, 1, 1, 1, 1, 0, 1, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 1, 1, 1, 1, 1, 1, 1, 0, 1 },
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
};

void RenderMaze(int maze1[ROW][COL], float tileSize, AEGfxVertexList* wallMesh, AEGfxVertexList* pathMesh, AEGfxVertexList* goalMesh, AEGfxVertexList* rockMesh) {
    (void)pathMesh;
    // Calculate the top-left corner of the maze
    float startX = 0.0f; // X starts at 0
    float startY = tileSize; // Y starts at the top (10 tiles * tile size)

    for (int y = 0; y < ROW; ++y) {
        for (int x = 0; x < COL; ++x) {
            // Calculate the position of each tile
            f32 posX = startX + (x * tileSize);
            f32 posY = startY - ((y + 1) * tileSize); // Subtract to flip the direction

            AEMtx33 transform = { 0 };
            AEMtx33Trans(&transform, posX, posY);
            AEGfxSetTransform(transform.m);
            // Draw walls or paths
            if (maze1[y][x] == 1) {
                AEGfxMeshDraw(wallMesh, AE_GFX_MDM_TRIANGLES);
            }
            if (maze1[y][x] == 2) {
                AEGfxMeshDraw(goalMesh, AE_GFX_MDM_TRIANGLES);
            }
            if (maze1[y][x] == 3) {
                AEGfxMeshDraw(rockMesh, AE_GFX_MDM_TRIANGLES);
            }
            //if (maze[y][x] == 0) {
            //    continue;
            //    AEMtx33 transform = { 0 };
            //    AEMtx33Trans(&transform, posX, posY);
            //    AEGfxSetTransform(transform.m);
            //    AEGfxMeshDraw(pathMesh, AE_GFX_MDM_TRIANGLES);
            //}
        }
    }
}



AEGfxVertexList* CreateSquareMesh(float size, u32 color)
{
    AEGfxMeshStart();

    // Add two triangles to form a square
    AEGfxTriAdd(-size / 2, -size / 2, color, 0.0f, 1.0f,
        size / 2, -size / 2, color, 1.0f, 1.0f,
        -size / 2, size / 2, color, 0.0f, 0.0f);

    AEGfxTriAdd(size / 2, -size / 2, color, 1.0f, 1.0f,
        size / 2, size / 2, color, 1.0f, 0.0f,
        -size / 2, size / 2, color, 0.0f, 0.0f);

    return AEGfxMeshEnd();
}


