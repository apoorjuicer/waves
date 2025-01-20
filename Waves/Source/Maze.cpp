#include "AEEngine.h"

void RenderMaze(int maze[10][10], float tileSize, AEGfxVertexList* wallMesh, AEGfxVertexList* pathMesh) {
    for (int y = 0; y < 10; ++y) {
        for (int x = 0; x < 10; ++x) {
            float posX = (x - 5.0f) * tileSize; // Adjust for new tile size
            float posY = (5.0f - y) * tileSize;

            if (maze[y][x] == 1) {
                AEMtx33 transform = { 0 };
                AEMtx33Trans(&transform, posX, posY);
                AEGfxSetTransform(transform.m);
                AEGfxMeshDraw(wallMesh, AE_GFX_MDM_TRIANGLES);
            }
            if (maze[y][x] == 0) {
                AEMtx33 transform = { 0 };
                AEMtx33Trans(&transform, posX, posY);
                AEGfxSetTransform(transform.m);
                AEGfxMeshDraw(pathMesh, AE_GFX_MDM_TRIANGLES);
            }
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
